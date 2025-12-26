#pragma once

#include "Server.hpp"

#include <boost/asio.hpp>
#include <boost/uuid.hpp>

class Session : public std::enable_shared_from_this<Session> {
    friend Server;
public:
    Session(Server* server);
    ~Session();

    void start();
    void unregister();

    static void HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, size_t recieve_lenth);
    static void HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec);

private:
    static const int MAX_LENGTH = 2048;
    boost::uuids::uuid uuid;
    Server* server;
    boost::asio::ip::tcp::socket socket;

    // tmp used data
    char* data;
};
