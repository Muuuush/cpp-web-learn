#pragma once

#include "PacketNode.hpp"

#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <sstream>

class Server;
class Session : public std::enable_shared_from_this<Session> {
    friend Server;
public:
    Session(Server* server);
    ~Session();

    std::optional<boost::system::system_error> send(TLVPacket node);
    [[nodiscard("Awaitable corountine")]]
    boost::asio::awaitable<std::optional<boost::system::system_error>> asyncSend(TLVPacket node);
    void close();

    boost::uuids::uuid getUUID() const noexcept { return uuid; }
    boost::asio::ip::tcp::endpoint getRemoteEndpoint() const noexcept { return socket.remote_endpoint(); }
    std::string toString() {
        std::stringstream ss;
        ss << socket.remote_endpoint() << " [" << uuid << "]";
        return ss.str();
    }

private:
    boost::asio::awaitable<void>startReceiving();

private:
    boost::uuids::uuid uuid;
    Server* server;
    boost::asio::ip::tcp::socket socket;
};
