#include "Client.hpp"
#include <iostream>

using namespace boost::asio;

Client::Client() : ioc(), socket(ioc) {}

void Client::connect(boost::asio::ip::tcp::endpoint serverEP) {
    socket.connect(serverEP);
}

void Client::connect(boost::asio::ip::tcp::endpoint serverEP, boost::system::error_code& ec) noexcept {
    socket.connect(serverEP, ec);
}

void Client::sendPacket(const TLVPacket& node) {
    write(
        socket,
        buffer(node.data, node.HEADER_SECTION + node.getLength())
    );
}

void Client::sendPacket(const TLVPacket& node, boost::system::error_code& ec) {
    write(
        socket,
        buffer(node.data, node.HEADER_SECTION + node.getLength()),
        ec
    );
}

TLVPacket Client::recievePacket() {
    TLVPacket node;
    read(
        this->socket,
        buffer(node.data, node.HEADER_SECTION)
    );
    read(
        this->socket,
        buffer(node.data + node.HEADER_SECTION, node.getLength())
    );
    return node;
}

TLVPacket Client::recievePacket(boost::system::error_code& ec) {
    TLVPacket node;
    read(
        this->socket,
        buffer(node.data, node.HEADER_SECTION),
        ec
    );
    if (ec.failed()) return TLVPacket();

    read(
        this->socket,
        buffer(node.data + node.HEADER_SECTION, node.getLength()),
        ec
    );
    if (ec.failed()) return TLVPacket();

    return node;
}
