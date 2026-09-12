#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace net {

/// Blocking IPv4 TCP client.
///
/// Lifecycle of a client socket: socket() -> connect() -> send()/recv() -> close()
/// The socket is closed automatically by the destructor (RAII).
class TCPClient {
 public:
  TCPClient(std::string host, std::uint16_t port);
  ~TCPClient();

  TCPClient(const TCPClient&) = delete;
  TCPClient& operator=(const TCPClient&) = delete;

  /// Connects to the server. Returns false if the connection is refused.
  /// Throws std::invalid_argument for a malformed address and
  /// std::system_error if the socket cannot be created.
  bool connect();

  /// Sends every byte of `message`. Throws std::system_error on failure.
  void send(std::string_view message) const;

  /// Blocks until data arrives. Returns an empty string when the server has
  /// closed the connection. Throws std::system_error on failure.
  std::string receive() const;

  void close();

  bool isConnected() const { return fd_ >= 0; }
  const std::string& host() const { return host_; }
  std::uint16_t port() const { return port_; }

 private:
  std::string host_;
  std::uint16_t port_;
  int fd_{-1};
};

}  // namespace net
