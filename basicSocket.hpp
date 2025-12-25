#pragma once

/*
 * 网络编程的基本流程
 *
 * server:
 * 1. create acceptor object (it is socket too)
 * 2. bind local ip and port
 * 3. accept -- create and assign a new socket to receive and send message
 * 4. read/write
 *
 * client:
 * 1. create socket object
 * 2. connect -- send connect request to server by ip and port
 * 3. write/read
 */
#include <boost/asio.hpp>

// end point -> ip + port
boost::asio::ip::tcp::endpoint clientEndPoint();
boost::asio::ip::tcp::endpoint serverEndPoint();

/* create socket
 * 1. create context
 * 2. choose protocol
 * 3. create socket
 * 4. open socket (check if it is valid, deprecated in newer version)
 */
boost::asio::ip::tcp::socket serverCreateTcpSocket();
// new version (auto bind)
boost::asio::ip::tcp::acceptor serverCreateAcceptorSocket();
// old version of acceptor creating
[[deprecated("Old version, use createAcceptorSocket() instead")]]
void serverBindAcceptorSocket();

void clientConnectToServer();
boost::asio::ip::tcp::endpoint ResolveHostByDNS();

void serverAcceptNewConnection();
