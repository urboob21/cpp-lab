// -----------------------------------------------------------------------------
// Server and client in one process
//
// The same TCPServer and TCPClient classes as in SimpleTCPServer and
// SimpleTCPClient, but both run inside this example: the server serves on a
// background thread and the client talks to it over the loopback interface.
// No second terminal is needed, so this example also runs in the smoke tests.
//
// Things to notice:
//   - Port 0 asks the operating system for any free port; boundPort() tells
//     which one was chosen (so the example never collides with other servers).
//   - open() binds and listens BEFORE the thread starts, so the client can
//     connect right away: connections queue up in the listen backlog.
//   - stop() wakes up the blocking accept() so the server thread can finish.
// -----------------------------------------------------------------------------

#include <string>
#include <thread>

#include "TCPClient.h"
#include "TCPServer.h"
#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

std::string trimmed(std::string text) {
  while (!text.empty() && (text.back() == '\n' || text.back() == '\r')) {
    text.pop_back();
  }
  return text;
}

void run() {
  LOG_SECTION("Starting the server on a free port");
  net::TCPServer server{0};
  server.open();
  std::thread server_thread([&server] { server.serve(); });

  LOG_SECTION("Talking to it with a client");
  net::TCPClient client{"127.0.0.1", server.boundPort()};
  if (client.connect()) {
    LOG_S("client got: " << trimmed(client.receive()));
    for (const std::string message :
         {"hello", "sockets are file descriptors", "Q"}) {
      client.send(message + "\n");
      LOG_S("client sent \"" << message << "\", echo: \""
                             << trimmed(client.receive()) << '"');
    }
    const std::string after_quit = client.receive();
    LOG_S("after Q the server closed the connection: " << std::boolalpha
                                                       << after_quit.empty());
  }

  LOG_SECTION("Stopping the server");
  server.stop();
  server_thread.join();
}

}  // namespace

LAB_EXAMPLE("LoopbackEcho",
            "TCP server thread and client in one process over 127.0.0.1") {
  run();
}
