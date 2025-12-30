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

awaitable<void> Session::startReceiving() {
    try {
        while (true) {
            TLVPacket node;
            co_await async_read(socket, buffer(node.data, node.HEADER_SECTION), use_awaitable);

            uint16_t tag = node.getTag();
            if (!LogicSystem::getCallbacks().contains(tag)) {
                throw std::runtime_error("Unknown tag packet");
            } else if (node.getLength() > node.MAX_LENGTH) {
                throw std::runtime_error("Length exceeding packet");
            }

            co_await async_read(socket, buffer(node.data + node.HEADER_SECTION, node.getLength()), use_awaitable);
            LogicSystem::getInstance(server->logicQueueCapacity, server->logicWorkerNum)
                ->registerNode(LogicNode(shared_from_this(), std::move(node)));
        }
    } catch (const boost::system::system_error& e) {
        if (e.code() == error::eof) {
            spdlog::debug("Connection closed by peer. {}", toString());
        } else {
            spdlog::error("{}, from {}", e.what(), toString());
        }
    } catch (const std::exception& e) {
        spdlog::error("{}, from {}", e.what(), toString());
    }
    this->close();
}

std::optional<boost::system::system_error> Session::send(TLVPacket node) {
    try {
        write(socket, buffer(node.data, node.HEADER_SECTION + node.getLength()));
    } catch (const boost::system::system_error& e) {
        this->close();
        return e;
    }
    return std::nullopt;
}

awaitable<std::optional<boost::system::system_error>> Session::asyncSend(TLVPacket node) {
    try {
        co_await async_write(socket, buffer(node.data, node.HEADER_SECTION + node.getLength()), use_awaitable);
    } catch (const boost::system::system_error& e) {
        this->close();
        co_return e;
    }
    co_return std::nullopt;
}

void Session::close() {
    server->sessions.erase(uuid);
}
