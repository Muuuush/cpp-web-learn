#include "Session.hpp"

#include "Server.hpp"
#include "LogicSystem.hpp"
#include "IOContextPool.hpp"
#include <spdlog/spdlog.h>

using namespace boost::asio;

Session::Session(Server* server)
    : uuid(boost::uuids::random_generator()()),
    server(server),
    socket(IOContextPool::getInstance(server->ioContextPoolSize)->getIOContext()) {}

Session::~Session() {}

void Session::startRecieving() {
    auto node = std::make_shared<TLVPacket>();
    async_read(
        this->socket,
        buffer(node->data, node->HEADER_SECTION),
        std::bind(
            &Session::HandleHead,
            shared_from_this(),
            std::placeholders::_1,
            node
        )
    );
}

void Session::send(TLVPacket node) {
    async_write(
        socket,
        buffer(node.data, node.HEADER_SECTION + node.getLength()),
        std::bind(
            &Session::HandleSend,
            shared_from_this(),
            std::placeholders::_1
        )
    );
}

void Session::close() {
    server->sessions.erase(uuid);
}

void Session::HandleHead(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<TLVPacket> node) {
    if (ec.failed()) {
        if (ec == error::eof)
            spdlog::debug("Connection closed by peer. ({})", session->toString());
        else
            spdlog::error("{}, from {}", ec.what(), session->toString());
        session->close();
    } else {
        uint16_t tag = node->getTag();

        if (!LogicSystem::getCallbacks().contains(tag)) {
            spdlog::error("Unknown type: {}, from {}", tag, session->toString());
            session->close();
        } else if (node->getLength() > node->MAX_LENGTH) {
            spdlog::warn("Length exceeding message from {}, the session will be closed.", ec.what(), session->toString());
            session->close();
        } else {
            async_read(
                session->socket,
                buffer(node->data + node->HEADER_SECTION, node->getLength()),
                std::bind(
                    &Session::HandleRecieve,
                    session,
                    std::placeholders::_1,
                    node
                )
            );
        }
    }
}

void Session::HandleRecieve(std::shared_ptr<Session> session, const boost::system::error_code& ec, std::shared_ptr<TLVPacket> node) {
    if (ec.failed()) {
        if (ec == error::eof)
            spdlog::debug("Connection closed by peer. ({})", session->toString());
        else
            spdlog::error("{}, from {}", ec.what(), session->toString());
        session->close();
    } else {
        LogicSystem::getInstance(session->server->logicQueueCapacity, session->server->logicWorkerNum)
            ->registerNode(LogicNode(session, std::move(*node)));
        session->startRecieving();
    }
}

void Session::HandleSend(std::shared_ptr<Session> session, const boost::system::error_code& ec) {
    if (ec.failed()) {
        spdlog::error("{}, from {}", ec.what(), session->toString());
        session->close();
    } else {
        // send successfully
    }
}
