#include "Session.hpp"

#include <iostream>

using namespace boost::asio;

Session::Session(Server* server)
    : uuid(boost::uuids::random_generator()()),
    server(server), socket(server->ioc) {
    // std::cout << "Session " << boost::uuids::to_string(uuid) << " created" << std::endl;
}

void Session::start() {
    data = new char[MAX_LENGTH]{};
    this->socket.async_read_some(
        buffer(this->data, MAX_LENGTH),
        std::bind(
            &Session::HandleRecieve,
            shared_from_this(),
            std::placeholders::_1,
            std::placeholders::_2
        )
    );
}

Session::~Session() {
    // std::cout << "Session " << boost::uuids::to_string(uuid) << " deleted" << std::endl;
}

void Session::unregister() {
    server->sessions.erase(uuid);
}

void Session::HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, size_t recieve_lenth)
{
    if (ec.failed()) {
        if (ec == error::eof)
            std::cout << "Connection closed by peer" << std::endl;
        else
            std::cerr << "[Error]: " << ec.what() << std::endl;
        session->unregister();
    } else {
        std::cout << "[Recieve]: " << std::string(session->data, recieve_lenth) << std::endl;

        session->socket.async_send(
            buffer(session->data, recieve_lenth),
            std::bind(
                &Session::HandleSend,
                session,
                std::placeholders::_1
            )
        );
    }
}

void Session::HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec) {
    if (ec.failed()) {
        std::cerr << "[Error]: " << ec.what() << std::endl;
        session->unregister();
    } else {
        std::memset(session->data, 0, sizeof(char) * MAX_LENGTH);
        session->socket.async_read_some(
            buffer(session->data, MAX_LENGTH),
            std::bind(
                &Session::HandleRecieve,
                session,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );
    }
}
