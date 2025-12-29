#pragma once

#include "PacketNode.hpp"
#include <boost/asio.hpp>

class Client {
public:
    Client();
    void connect(boost::asio::ip::tcp::endpoint serverEP);
    void connect(boost::asio::ip::tcp::endpoint serverEP, boost::system::error_code& ec) noexcept;
    void sendPacket(const TLVPacket& node);
    void sendPacket(const TLVPacket& node, boost::system::error_code& ec);
    TLVPacket recievePacket();
    TLVPacket recievePacket(boost::system::error_code& ec);

private:
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket socket;
};
