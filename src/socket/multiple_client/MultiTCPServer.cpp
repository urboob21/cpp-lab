// -----------------------------------------------------------------------------
// Multi-client TCP echo server: one thread per connection
//
// SimpleTCPServer serves clients one after another. Here accept() hands every
// new connection to its own std::thread, so a slow client no longer blocks the
// others.
//
// Things to notice:
//   - Shared state (the list of connected sockets) is protected by a mutex.
//   - On shutdown the remaining client sockets are shut down, which wakes up
//     their threads blocked in recv(), so all threads can be joined.
//   - One thread per client does not scale to thousands of connections;
//     real servers use thread pools or event loops (epoll, io_uring, asio).
//
// Try it: run this example, then connect from several terminals with
//   telnet localhost 8080
// Send SHUTDOWN from any client to stop the server.
// -----------------------------------------------------------------------------

#include "MultiTCPServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <exception>

#include "lab/Example.h"
#include "lab/Logger.h"

namespace net {

MultiTCPServer::~MultiTCPServer() {
  stop();
  disconnectAllClients();
  for (std::thread& thread : client_threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void MultiTCPServer::acceptLoop() {
  while (isRunning()) {
    sockaddr_in client_address{};
    socklen_t length = sizeof(client_address);
    const int client_fd = ::accept(
        serverFd(), reinterpret_cast<sockaddr*>(&client_address), &length);
    if (client_fd < 0) {
      if (!isRunning()) {
        break;
      }
      if (errno != EINTR) {
        LOG_S("[MultiTCPServer] accept failed: " << std::strerror(errno));
      }
      continue;
    }

    {
      const std::lock_guard<std::mutex> lock(clients_mutex_);
      client_fds_.push_back(client_fd);
      LOG_S("[MultiTCPServer] client fd " << client_fd << " connected ("
                                          << client_fds_.size()
                                          << " connected)");
    }

    client_threads_.emplace_back([this, client_fd] {
      try {
        handleClient(client_fd);
      } catch (const std::exception& e) {
        LOG_S("[MultiTCPServer] client error: " << e.what());
      }
      {
        // Forget the fd BEFORE closing it, so disconnectAllClients() can never
        // touch a number the OS has already reused for another socket.
        const std::lock_guard<std::mutex> lock(clients_mutex_);
        std::erase(client_fds_, client_fd);
      }
      ::close(client_fd);
    });
  }

  disconnectAllClients();
  for (std::thread& thread : client_threads_) {
    thread.join();
  }
  client_threads_.clear();
}

void MultiTCPServer::disconnectAllClients() {
  const std::lock_guard<std::mutex> lock(clients_mutex_);
  for (const int fd : client_fds_) {
    ::shutdown(fd, SHUT_RDWR);  // recv() in the client thread returns 0
  }
}

}  // namespace net

LAB_EXAMPLE("MultiTCPServer",
            "echo server on 127.0.0.1:8080 with one thread per client",
            lab::kInteractive) {
  net::MultiTCPServer server{8080};
  try {
    server.start();  // blocks until a client sends SHUTDOWN
  } catch (const std::exception& e) {
    LOG_S("server error: " << e.what());
  }
}
