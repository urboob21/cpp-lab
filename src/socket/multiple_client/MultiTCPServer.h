#pragma once

#include <mutex>
#include <thread>
#include <vector>

#include "../simple_tcp/TCPServer.h"

namespace net {

/// Echo server that serves every client on its own thread, so several clients
/// can be connected at the same time ("thread per connection").
class MultiTCPServer : public TCPServer {
 public:
  using TCPServer::TCPServer;
  ~MultiTCPServer() override;

 protected:
  void acceptLoop() override;

 private:
  void disconnectAllClients();

  std::vector<std::thread> client_threads_;
  std::mutex clients_mutex_;
  std::vector<int> client_fds_;  // sockets of the clients still connected
};

}  // namespace net
