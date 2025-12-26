#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

class Server;
/*
 * 存在二次释放的风险
 * 如果客户端在服务器发送数据是关闭
 * - handleRecieve 会收到 eof 错误码销毁对象
 * - handleSend 会收到错误销毁对象
 */
class Session {
    friend Server;
public:
    Session(io_context& ioc) : socket(ioc), data((char*)calloc(sizeof(char), MAX_LENGTH)) {}

    void handleRecieve(const boost::system::error_code& ec, size_t recieve_length) {
        if (!ec.failed()) {
            std::cout << "[Recieve]: " << data << std::endl;
            async_write(
                socket,
                buffer(data, recieve_length),
                std::bind(
                    &Session::handleSend,
                    this,
                    std::placeholders::_1
                )
            );
        } else {
            if (ec == error::eof)
                std::cout << "Connection closed by peer" << std::endl;
            else
                std::cerr << "[Error] when recieve: " << ec.value() << std::endl;
            delete this;
        }
    }

    void handleSend(const boost::system::error_code& ec) {
        if (!ec.failed()) {
            memset(data, 0, MAX_LENGTH * sizeof(char));
            socket.async_read_some(
                buffer(data, MAX_LENGTH),
                std::bind(
                    &Session::handleRecieve,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                )
            );
        } else {
            std::cerr << "[Error] when send: " << ec.value() << std::endl;
            delete this;
        }
    }

private:
    static const int MAX_LENGTH = 1024;
    ip::tcp::socket socket;
    char* data;
};

class Server {
public:
    Server(io_context& ioc, unsigned short port)
        : ioc(ioc), port(port), acceptor(ioc, ip::tcp::endpoint(ip::tcp::v4(), port))
    {
        acceptor.listen(BACKLOG);
        std::cout << "[Listening ...]" << std::endl;
    }

    void startAccept() {
        auto session = new Session(ioc);
        auto handleAccept = [session, this](const boost::system::error_code& ec) {
            if (ec.failed()) {
                std::cerr << "[Error]: " << ec.to_string() << std::endl;
            } else {
                auto client = session->socket.remote_endpoint();
                std::cout << "Connected to " << client.address().to_string() << ":" << client.port() << std::endl;
                session->socket.async_read_some(
                    buffer(session->data, Session::MAX_LENGTH),
                    std::bind(
                        &Session::handleRecieve,
                        session,
                        std::placeholders::_1,
                        std::placeholders::_2
                    )
                );
            }
            this->startAccept();
        };
        acceptor.async_accept(session->socket, handleAccept);
    }

private:
    static const int BACKLOG = 30;
    io_context& ioc;
    const unsigned short port;
    ip::tcp::acceptor acceptor;
};

int main() {
    try {
        io_context ioc;
        Server server(ioc, 10086);
        server.startAccept();
        ioc.run();
    } catch (const boost::system::system_error& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }
    return 0;
}
