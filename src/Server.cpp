#include "Server.hpp"

#include "Session.hpp"
#include "LogicSystem.hpp"
#include "IOContextPool.hpp"
#include <spdlog/spdlog.h>

using namespace boost::asio;

Server::Server(unsigned short port, ServerSetting setting)
    : ioc(), port(port), backlog(setting.backlog),
    ioContextPoolSize(setting.ioContextPoolSize),
    logicQueueCapacity(setting.logicQueueCapacity), logicWorkerNum(setting.logicWorkerNum),
    acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port)),
    stopCallback(setting.stopCallback)
{}

void Server::start() {
    spdlog::info("Server start at port: {}", port);
    acceptor.listen(backlog);
    boost::asio::signal_set signals(ioc, SIGINT, SIGTERM);
    auto stopServer = [this] {
        ioc.stop();
        IOContextPool::destoryInstance();
        LogicSystem::destoryInstance();
        if (stopCallback) stopCallback();
    };
    signals.async_wait([&](auto, auto) { stopServer(); });
    try {
        startAccept();
        ioc.run();
    } catch (const std::exception& e) {
        spdlog::critical(e.what());
        stopServer();
    }
}

void Server::startAccept() {
    auto newSession = std::make_shared<Session>(this);
    acceptor.async_accept(
        newSession->socket,
        [newSession, this](const boost::system::error_code& ec) {
            this->sessions.insert({newSession->uuid, newSession});
            auto ep = newSession->getRemoteEndpoint();
            spdlog::debug("{} connected.", newSession->toString());
            co_spawn(newSession->socket.get_executor(), newSession->startReceiving(), detached);

            startAccept();
        }
    );
}
