#pragma once

#include <boost/asio.hpp>
#include <boost/uuid.hpp>
#include <unordered_map>

class Session;

struct ServerSetting {
    int backlog = boost::asio::socket_base::max_listen_connections;
    int ioContextPoolSize = std::thread::hardware_concurrency();
    int logicQueueCapacity = 1024;
    int logicWorkerNum = 1;
    std::function<void()> stopCallback;
    ServerSetting() = default;
};

class Server {
    friend Session;
public:
    Server(unsigned short port, ServerSetting setting = {});
    void start();
private:
    void startAccept();

private:
    boost::asio::io_context ioc;
    const unsigned short port;
    int backlog;
    const int ioContextPoolSize;
    const int logicQueueCapacity;
    const int logicWorkerNum;
    boost::asio::ip::tcp::acceptor acceptor;
    std::unordered_map<boost::uuids::uuid, std::shared_ptr<Session>> sessions;

    std::function<void()> stopCallback;
};
