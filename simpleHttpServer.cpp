#include "simpleHttpServer.h"
#include <iostream>
#include <sstream>

namespace {
void log(const std::string &message) { std::cout << message << std::endl; }
} // namespace

namespace server {
TcpServer::TcpServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(),
      m_incoming_message(), m_socket_address(),
      m_socket_address_len(sizeof(m_socket_address)),
      m_server_message(build_response()) {
  m_socket_address.sin_family = AF_INET;
  m_socket_address.sin_port = htons(m_port);
  m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

  if (start_server() != 0) {
    std::ostringstream ss;
    ss << "Failed to start server with PORT: "
       << ntohs(m_socket_address.sin_port);
    log(ss.str());
  }
}
} // namespace server
