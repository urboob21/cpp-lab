#include "TCPClient.h"

#include <arpa/inet.h>   // inet_pton, htons
#include <netinet/in.h>  // sockaddr_in
#include <sys/socket.h>  // socket, connect, send, recv
#include <unistd.h>      // close

#include <array>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <utility>

#include "lab/Logger.h"

namespace net {

TCPClient::TCPClient(std::string host, std::uint16_t port)
    : host_{std::move(host)}, port_{port} {}

TCPClient::~TCPClient() {
  close();
}

bool TCPClient::connect() {
  close();  // reconnecting: release a previous socket first

  sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  // inet_pton converts "127.0.0.1" into the binary form (1 = ok, 0 = invalid).
  if (::inet_pton(AF_INET, host_.c_str(), &server_address.sin_addr) != 1) {
    throw std::invalid_argument("invalid IPv4 address: " + host_);
  }

  fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    throw std::system_error(errno, std::generic_category(), "socket");
  }

  // `::connect` is the POSIX function; plain `connect` would be this member.
  if (::connect(fd_, reinterpret_cast<sockaddr*>(&server_address),
                sizeof(server_address)) < 0) {
    LOG_S("[TCPClient] connect to " << host_ << ':' << port_
                                    << " failed: " << std::strerror(errno));
    close();
    return false;
  }
  return true;
}

void TCPClient::send(std::string_view message) const {
  if (fd_ < 0) {
    throw std::logic_error("TCPClient::send: not connected");
  }
  std::size_t total = 0;
  while (total < message.size()) {
    const ssize_t sent = ::send(fd_, message.data() + total,
                                message.size() - total, MSG_NOSIGNAL);
    if (sent < 0) {
      if (errno == EINTR) {
        continue;
      }
      throw std::system_error(errno, std::generic_category(), "send");
    }
    total += static_cast<std::size_t>(sent);
  }
}

std::string TCPClient::receive() const {
  if (fd_ < 0) {
    throw std::logic_error("TCPClient::receive: not connected");
  }
  std::array<char, 1024> buffer{};
  while (true) {
    const ssize_t received = ::recv(fd_, buffer.data(), buffer.size(), 0);
    if (received < 0) {
      if (errno == EINTR) {
        continue;
      }
      throw std::system_error(errno, std::generic_category(), "recv");
    }
    return std::string(buffer.data(),
                       static_cast<std::size_t>(received));  // "" = closed
  }
}

void TCPClient::close() {
  if (fd_ >= 0) {
    ::close(fd_);
    fd_ = -1;
  }
}

}  // namespace net
