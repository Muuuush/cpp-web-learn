#include "Server.hpp"

#include "Session.hpp"

using namespace boost::asio;

Server::Server(boost::asio::io_context& ioc, unsigned short port)
    : ioc(ioc), port(port),
    acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port)) {}

void Server::start() {
    acceptor.listen(BACKLOG);
    startAccept();
}

void Server::startAccept() {
    auto newSession = std::make_shared<Session>(this);
    acceptor.async_accept(
        newSession->socket,
        [newSession, this](const boost::system::error_code& ec) {
            this->sessions.insert({newSession->uuid, newSession});
            newSession->start();

            startAccept();
        }
    );
}
