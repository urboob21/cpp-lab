#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace net {

/// A small, blocking, line-based TCP echo server for IPv4 loopback.
///
/// Lifecycle of a server socket:
///   socket() -> setsockopt() -> bind() -> listen() -> accept() loop
///   -> recv()/send() with each client -> close()
///
/// Protocol: every received byte is echoed back. A line "Q" closes the
/// connection, a line "SHUTDOWN" stops the whole server.
class TCPServer {
 public:
  /// Port 0 lets the operating system pick a free port (see boundPort()).
  explicit TCPServer(std::uint16_t port = 8080);
  virtual ~TCPServer();

  TCPServer(const TCPServer&) = delete;
  TCPServer& operator=(const TCPServer&) = delete;

  /// open() + serve(): blocks until stop() is called.
  void start();

  /// Creates the socket, binds it and starts listening. Throws std::system_error.
  void open();

  /// Accepts and serves clients until stop() is called.
  void serve();

  /// Stops the accept loop. Safe to call from another thread.
  void stop();

  bool isRunning() const { return running_; }

  /// The port actually in use (differs from the requested one for port 0).
  std::uint16_t boundPort() const { return bound_port_; }

 protected:
  /// Serves clients one after another. MultiTCPServer overrides it.
  virtual void acceptLoop();

  /// Echo protocol for one connected client. Does not close `client_fd`.
  void handleClient(int client_fd);

  /// Sends every byte, retrying on partial sends. Throws std::system_error.
  static void sendAll(int fd, std::string_view data);

  int serverFd() const { return server_fd_; }

 private:
  void createSocket();
  void bindSocket();
  void listenSocket() const;
  void closeServerSocket();

  std::uint16_t requested_port_;
  std::uint16_t bound_port_{0};
  std::atomic<int> server_fd_{-1};
  std::atomic<bool> running_{false};
};

}  // namespace net
