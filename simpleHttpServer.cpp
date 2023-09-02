#include "simpleHttpServer.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace {
const int BUFFER_SIZE{30720};
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

TcpServer::~TcpServer() {
  close(m_socket);
  close(m_new_socket);
  exit(0);
}

void TcpServer::start_listen() {
  if (listen(m_socket, 20) < 0) {
    exit_with_error("Socket listen failed.");
  }

  std::ostringstream ss;
  ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socket_address.sin_addr)
     << " PORT: " << ntohs(m_socket_address.sin_port) << " ***\n\n";
  log(ss.str());
  int bytes_received = 0;

  while (true) {
    log("===== Waiting for new connection =====\n\n\n");
    accept_connection(m_new_socket);

    char buffer[BUFFER_SIZE] = {0};
    bytes_received = read(m_new_socket, buffer, BUFFER_SIZE);
    if (bytes_received < 0) {
      exit_with_error("Failed to receive bytes from client socket connection");
    }
    std::ostringstream ss;
    ss << "------ Received bytes from client ------\n\n";
    log(ss.str());
  }

  send_response();

  close(m_new_socket);
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

void TcpServer::accept_connection(int &new_socket) {
  new_socket =
      accept(m_socket, (sockaddr *)&m_socket_address, &m_socket_address_len);
  if (new_socket < 0) {
    std::ostringstream ss;
    ss << "Server failed to accept connection from ADDRESS: "
       << inet_ntoa(m_socket_address.sin_addr)
       << "; PORT: " << ntohs(m_socket_address.sin_port);
    exit_with_error(ss.str());
  }
}

std::string TcpServer::build_response() {
  std::string html_file =
      "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from "
      "your Server :) </p></body></html>";
  std::ostringstream ss;
  ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-length: "
     << html_file.size() << "\n\n"
     << html_file;

  return ss.str();
}

void TcpServer::send_response() {
  long bytes_sent;

  bytes_sent =
      write(m_new_socket, m_server_message.c_str(), m_server_message.size());

  if (bytes_sent == m_server_message.size()) {
    log("------ Server Response sent to client ------\n\n");
  } else {
    log("error sending Response to client");
  }
}

} // namespace server
