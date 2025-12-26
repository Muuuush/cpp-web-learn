#pragma once

#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <unordered_map>

class Session;

class Server {
    friend Session;
public:
    Server(boost::asio::io_context& ioc, unsigned short port);
    void start();
private:
    void startAccept();

private:
    static const int BACKLOG = 30;
    boost::asio::io_context& ioc;
    const unsigned short port;
    boost::asio::ip::tcp::acceptor acceptor;
    std::unordered_map<boost::uuids::uuid, std::shared_ptr<Session>> sessions;
};
