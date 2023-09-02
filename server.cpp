#include "simpleHttpServer.h"

int main(int argc, char *argv[]) {
  server::TcpServer tcp_server{"0.0.0.0", 5400};
  tcp_server.start_listen();

  return 0;
}
