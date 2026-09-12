#include "TCPServer.h"

#include <arpa/inet.h>   // inet_ntop, htons, htonl
#include <netinet/in.h>  // sockaddr_in, INADDR_LOOPBACK
#include <sys/socket.h>  // socket, bind, listen, accept, recv, send, shutdown
#include <unistd.h>      // close

#include <array>
#include <cerrno>
#include <cstring>
#include <string>
#include <system_error>

#include "lab/Logger.h"

namespace net {
namespace {

/// Wraps errno in an exception, e.g. "bind: Address already in use".
[[noreturn]] void throwSystemError(const char* operation) {
  throw std::system_error(errno, std::generic_category(), operation);
}

std::string toString(const sockaddr_in& address) {
  std::array<char, INET_ADDRSTRLEN> text{};
  // inet_ntop is the thread-safe replacement for inet_ntoa.
  ::inet_ntop(AF_INET, &address.sin_addr, text.data(), text.size());
  return std::string(text.data()) + ":" +
         std::to_string(ntohs(address.sin_port));
}

std::string_view withoutLineEnd(std::string_view text) {
  while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
    text.remove_suffix(1);
  }
  return text;
}

}  // namespace

TCPServer::TCPServer(std::uint16_t port) : requested_port_{port} {}

TCPServer::~TCPServer() {
  closeServerSocket();
}

void TCPServer::start() {
  open();
  serve();
}

void TCPServer::open() {
  if (server_fd_ >= 0) {
    return;  // already open
  }
  createSocket();
  bindSocket();
  listenSocket();
  running_ = true;
  LOG_S("[TCPServer] listening on 127.0.0.1:" << bound_port_);
}

void TCPServer::serve() {
  acceptLoop();
  LOG("[TCPServer] stopped");
}

void TCPServer::stop() {
  if (running_.exchange(false)) {
    // shutdown() on a listening socket makes a blocked accept() return an
    // error, which lets acceptLoop() notice that it should exit.
    const int fd = server_fd_;
    if (fd >= 0) {
      ::shutdown(fd, SHUT_RDWR);
    }
  }
}

void TCPServer::createSocket() {
  // IPv4 (AF_INET) + reliable byte stream (SOCK_STREAM = TCP)
  const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    throwSystemError("socket");
  }
  server_fd_ = fd;  // from here on the destructor closes it

  // Allow restarting the server immediately. Without SO_REUSEADDR the port
  // stays blocked in TIME_WAIT for up to a few minutes after the server stops.
  const int reuse = 1;
  if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    throwSystemError("setsockopt(SO_REUSEADDR)");
  }
}

void TCPServer::bindSocket() {
  sockaddr_in address{};
  address.sin_family = AF_INET;
  // 127.0.0.1 accepts connections from this machine only. INADDR_ANY would
  // listen on every network interface (loopback, LAN, Wi-Fi, ...).
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  address.sin_port = htons(requested_port_);  // host -> network byte order

  if (::bind(server_fd_, reinterpret_cast<sockaddr*>(&address),
             sizeof(address)) < 0) {
    throwSystemError("bind");  // e.g. "Address already in use"
  }

  // Ask the OS which port we really got (relevant for port 0).
  socklen_t length = sizeof(address);
  if (::getsockname(server_fd_, reinterpret_cast<sockaddr*>(&address),
                    &length) < 0) {
    throwSystemError("getsockname");
  }
  bound_port_ = ntohs(address.sin_port);
}

void TCPServer::listenSocket() const {
  // SOMAXCONN: maximum length of the queue of pending connections.
  if (::listen(server_fd_, SOMAXCONN) < 0) {
    throwSystemError("listen");
  }
}

void TCPServer::acceptLoop() {
  while (running_) {
    sockaddr_in client_address{};
    socklen_t length = sizeof(client_address);

    // Blocks until a client connects (or stop() is called).
    const int client_fd = ::accept(
        server_fd_, reinterpret_cast<sockaddr*>(&client_address), &length);
    if (client_fd < 0) {
      if (!running_) {
        break;
      }
      if (errno == EINTR) {
        continue;  // interrupted by a signal: just try again
      }
      LOG_S("[TCPServer] accept failed: " << std::strerror(errno));
      continue;
    }

    LOG_S("[TCPServer] client " << toString(client_address) << " connected");
    try {
      handleClient(client_fd);
    } catch (const std::exception& e) {
      LOG_S("[TCPServer] client error: " << e.what());
    }
    ::close(client_fd);  // always release the client socket
  }
}

void TCPServer::handleClient(int client_fd) {
  sendAll(client_fd,
          "Connected to the cpp-lab echo server on port " +
              std::to_string(bound_port_) +
              "\nType Q to disconnect or SHUTDOWN to stop the server.\n");

  std::array<char, 1024> buffer{};
  std::string pending;  // bytes of a line that is not complete yet

  while (true) {
    const ssize_t received = ::recv(client_fd, buffer.data(), buffer.size(), 0);
    if (received == 0) {
      LOG("[TCPServer] client closed the connection");
      return;
    }
    if (received < 0) {
      if (errno == EINTR) {
        continue;
      }
      throwSystemError("recv");
    }

    const std::string_view chunk(buffer.data(),
                                 static_cast<std::size_t>(received));
    LOG_S("[TCPServer] fd " << client_fd << " received \""
                            << withoutLineEnd(chunk) << "\"");
    sendAll(client_fd, chunk);  // echo

    // TCP is a byte STREAM, not a sequence of messages: one recv() may contain
    // half a line or several lines. Collect bytes until a '\n' completes a line.
    pending.append(chunk);
    for (auto end = pending.find('\n'); end != std::string::npos;
         end = pending.find('\n')) {
      const std::string line(
          withoutLineEnd(std::string_view(pending).substr(0, end)));
      pending.erase(0, end + 1);
      if (line == "Q") {
        LOG("[TCPServer] client sent Q, closing its connection");
        return;
      }
      if (line == "SHUTDOWN") {
        LOG("[TCPServer] client sent SHUTDOWN, stopping the server");
        stop();
        return;
      }
    }
  }
}

void TCPServer::sendAll(int fd, std::string_view data) {
  std::size_t total = 0;
  while (total < data.size()) {
    // send() may transmit fewer bytes than requested. MSG_NOSIGNAL prevents a
    // SIGPIPE (which would kill the process) when the peer has disconnected.
    const ssize_t sent =
        ::send(fd, data.data() + total, data.size() - total, MSG_NOSIGNAL);
    if (sent < 0) {
      if (errno == EINTR) {
        continue;
      }
      throwSystemError("send");
    }
    total += static_cast<std::size_t>(sent);
  }
}

void TCPServer::closeServerSocket() {
  running_ = false;
  const int fd =
      server_fd_.exchange(-1);  // take the fd so it is closed only once
  if (fd >= 0) {
    ::close(fd);
  }
}

}  // namespace net
