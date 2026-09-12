// -----------------------------------------------------------------------------
// Simple TCP client
//
//   socket()   create an endpoint
//   connect()  open a connection to the server's address and port
//   send()     write bytes, recv() reads the reply
//   close()    end the connection
//
// Start SimpleTCPServer or MultiTCPServer in another terminal first, then type
// lines here. Each line is sent with a trailing '\n' because the server's
// protocol is line-based: std::getline removes the newline, and without it the
// server could not tell where a command such as "Q" ends. (telnet sends "\r\n".)
// -----------------------------------------------------------------------------

#include <exception>
#include <iostream>
#include <string>

#include "TCPClient.h"
#include "lab/Example.h"
#include "lab/Logger.h"

namespace {

void run() {
  net::TCPClient client{"127.0.0.1", 8080};
  if (!client.connect()) {
    LOG("Is the server running? Start SimpleTCPServer in another terminal "
        "first.");
    return;
  }
  LOG_S("connected to " << client.host() << ':' << client.port());
  std::cout << client.receive() << std::flush;  // welcome message

  std::string line;
  while (std::getline(std::cin, line)) {
    client.send(line + "\n");
    const std::string reply = client.receive();
    if (reply.empty()) {
      LOG("the server closed the connection");
      break;
    }
    std::cout << "echo: " << reply << std::flush;
    if (line == "Q" || line == "SHUTDOWN") {
      break;
    }
  }
}

}  // namespace

LAB_EXAMPLE("SimpleTCPClient",
            "type lines and send them to the echo server on 127.0.0.1:8080",
            lab::kInteractive) {
  try {
    run();
  } catch (const std::exception& e) {
    LOG_S("client error: " << e.what());
  }
}
