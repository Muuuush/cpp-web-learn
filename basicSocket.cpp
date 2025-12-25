#include "basicSocket.hpp"

#include <format>
#include <iostream>
#include <sys/wait.h>

using namespace boost;

static const std::string host = "sample.com";
static const std::string localhost = "127.0.0.1";
static const unsigned short clientPort = 3333;
static const unsigned short serverPort = 12345;

asio::ip::tcp::endpoint clientEndPoint() {
    asio::ip::address ipAddress = asio::ip::make_address(localhost);
    asio::ip::tcp::endpoint endPoint(ipAddress, clientPort);
    return endPoint;

    // if use error code
    // system::error_code errorCode;
    // asio::ip::address ipAddress = asio::ip::make_address(clientIP, errorCode);
    // if (errorCode.failed()) {
    //     std::cerr << std::format("Failed to parse the IP address. Error code: {}; Message: {}",
    //         errorCode.value(), errorCode.message()) << std::endl;
    //     return;
    // }
}

asio::ip::tcp::endpoint serverEndPoint() {
    // asio::ip::address ipAddress = asio::ip::address_v4::any();
    asio::ip::address ipAddress = asio::ip::make_address(localhost);
    asio::ip::tcp::endpoint endPoint(ipAddress, serverPort);
    return endPoint;
}

asio::ip::tcp::socket serverCreateTcpSocket() {
    asio::io_context ioContext;
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    asio::ip::tcp::socket socket(ioContext, protocol);
    return socket;
}

asio::ip::tcp::acceptor serverCreateAcceptorSocket() {
    asio::io_context ioContext;
    // auto bind creation
    asio::ip::tcp::acceptor acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 5555));
    return acceptor;
}

void serverBindAcceptorSocket() {
    asio::io_context ioContext;
    asio::ip::tcp::endpoint endPoint = serverEndPoint();
    asio::ip::tcp::acceptor acceptor(ioContext, endPoint.protocol());
    try {
        acceptor.bind(endPoint);
    } catch (boost::system::system_error& e) {
        std::cerr << std::format("Acceptor bind failed. Error code: {}; Error message: {}", e.code().value(), e.what()) << std::endl;
    }
}

void clientConnectToServer() {
    try {
        asio::ip::tcp::endpoint serverEP = serverEndPoint();
        asio::io_context ioContext;
        asio::ip::tcp::socket socket(ioContext, serverEP.protocol());
        socket.connect(serverEP);
    } catch (boost::system::system_error& e) {
        std::cerr << std::format("Connect to server failed. Error code: {}; Error message: {}", e.code().value(), e.what()) << std::endl;
    }
}

boost::asio::ip::tcp::endpoint ResolveHostByDNS() {
    asio::io_context ioContext;
    asio::ip::tcp::resolver resolver(ioContext);
    try {
        asio::ip::tcp::resolver::results_type results = resolver.resolve(host, "http", asio::ip::tcp::resolver::numeric_service);
        // for (const auto& entry : results) {
        //     std::cout << entry.endpoint().address() << ":"
        //               << entry.endpoint().port() << std::endl;
        // }
        return results.begin()->endpoint();
    } catch (boost::system::system_error& e) {
        std::cerr << std::format("Resolve host name failed. Error code: {}; Error message: {}", e.code().value(), e.what()) << std::endl;
    }
    return {};
}

void serverAcceptNewConnection() {
    const int BACKLOG_SIZE = 30;
    try {
        asio::io_context ioContext;
        auto endPoint = serverEndPoint();
        asio::ip::tcp::acceptor acceptor(ioContext, endPoint);
        acceptor.set_option(asio::socket_base::reuse_address(true));
        acceptor.listen(BACKLOG_SIZE);
        asio::ip::tcp::socket socket(ioContext);
        acceptor.accept(socket);
    } catch (boost::system::system_error& e) {
        std::cerr << std::format("Accept connection failed. Error code: {}; Error message: {}", e.code().value(), e.what()) << std::endl;
    }
}

int main() {
    if (pid_t child_pid = fork()) {
        serverAcceptNewConnection();
        std::cout << "Server accept successfully." << std::endl;
        waitpid(child_pid, NULL, 0);
    } else {
        sleep(1);
        clientConnectToServer();
        std::cout << "Client connect successfully." << std::endl;
    }
    return 0;
}
