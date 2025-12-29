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

    void send(TLVPacket node);
    void close();

    boost::uuids::uuid getUUID() const noexcept { return uuid; }
    boost::asio::ip::tcp::endpoint getRemoteEndpoint() const noexcept { return socket.remote_endpoint(); }
    std::string toString() {
        std::stringstream ss;
        ss << socket.remote_endpoint() << " [" << uuid << "]";
        return ss.str();
    }

private:
    void startRecieving();

    static void HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<TLVPacket> node);
    static void HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<TLVPacket> node);
    static void HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec);

private:
    boost::uuids::uuid uuid;
    Server* server;
    boost::asio::ip::tcp::socket socket;
};
