// -----------------------------------------------------------------------------
// Simple TCP echo server (see README.md in src/socket for the background)
//
//   socket()  create an endpoint
//   bind()    attach it to an address and port (127.0.0.1:8080)
//   listen()  mark it as passive: it waits for connections
//   accept()  block until a client connects, returns a NEW socket for it
//   recv()    read bytes from the client, send() writes bytes back
//   close()   release the client socket, and finally the server socket
//
// This server handles ONE client at a time: while it serves a client, other
// clients wait in the listen queue (see MultiTCPServer for a threaded server).
//
// Try it in a second terminal:
//   telnet localhost 8080      (or: nc localhost 8080)
//   or run the SimpleTCPClient example
// Type lines to get them echoed, Q to disconnect, SHUTDOWN to stop the server.
// -----------------------------------------------------------------------------

#include <exception>

#include "TCPServer.h"
#include "lab/Example.h"
#include "lab/Logger.h"

LAB_EXAMPLE("SimpleTCPServer",
            "echo server on 127.0.0.1:8080, one client at a time (telnet "
            "localhost 8080)",
            lab::kInteractive) {
  net::TCPServer server{8080};
  try {
    server.start();  // blocks until a client sends SHUTDOWN
  } catch (const std::exception& e) {
    LOG_S("server error: " << e.what());
  }
}
