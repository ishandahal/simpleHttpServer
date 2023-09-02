#ifndef SIMPLE_HTTP_SERVER
#define SIMPLE_HTTP_SERVER

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
namespace server {

class TcpServer {
public:
  TcpServer(std::string ip_address, int port);
  ~TcpServer();
  void start_listen();

private:
  std::string m_ip_address;
  std::string m_server_message;
  int m_port;
  int m_socket;
  int m_new_socket;
  long m_incoming_message;
  struct sockaddr_in m_socket_address;
  unsigned int m_socket_address_len;

  int start_server();
  void close_server();
  void accept_connection(int &new_socket);
  std::string build_response();
  void send_response();
};
} // namespace server

#endif // !SIMPLE_HTTP_SERVER
