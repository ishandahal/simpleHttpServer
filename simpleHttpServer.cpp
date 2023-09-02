#include "simpleHttpServer.h"
#include <iostream>
#include <sstream>

namespace {
void log(const std::string &message) { std::cout << message << std::endl; }
void exit_with_error(const std::string &error_msg) {
  log("Error: " + error_msg);
  exit(1);
}
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

int TcpServer::start_server() {
  m_socket = socket(
      AF_INET, SOCK_STREAM,
      0); // AF_INET for ipv4, SOCK_STREAM for reliable full_duples byte stream
  if (m_socket < 0) {
    exit_with_error("Cannot create socket.");
    return 1;
  }
  if (bind(m_socket, (sockaddr *)&m_socket_address, m_socket_address_len) < 0) {
    exit_with_error("Cannot connect socket with address.");
    return 1;
  }
  return 0;
}
} // namespace server
