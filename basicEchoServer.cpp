#include <iostream>
#include <thread>
#include <boost/asio.hpp>

using namespace boost::asio;

const int PORT = 10086;
const int BACKLOG = 30;
const int MAX_LENGTH = 1024;

void session(ip::tcp::socket* socket) {
    try {
        while (true) {
            char msg[MAX_LENGTH] = {};
            boost::system::error_code ec;
            size_t length = socket->read_some(buffer(msg, MAX_LENGTH), ec);
            if (ec == error::eof) {
                std::cout << "connection closed by peer" << std::endl;
                break;
            } else if (ec.failed()){
                throw boost::system::system_error(ec);
            }
            std::cout << "Message recieved" << std::endl;
            std::cout << "[From]: " << socket->remote_endpoint().address().to_string() << ":" << socket->remote_endpoint().port() << "\n\t" << msg << std::endl;
            write(*socket, buffer(msg, length));
        }
    } catch (const boost::system::system_error& e) {
        std::cout << "[Error]: " << socket->remote_endpoint().address().to_string() << ": " << socket->remote_endpoint().port() << "\n\t" << e.what() << std::endl;
    }
}

int main() {
    std::vector<std::shared_ptr<std::thread>> threads;
    io_context ioContext;
    ip::tcp::acceptor acceptor(ioContext, ip::tcp::endpoint(ip::tcp::v4(), PORT));
    try {
        std::cout << "Listening ..." << std::endl;
        acceptor.listen(BACKLOG);
        ip::tcp::socket* socket = new ip::tcp::socket(ioContext);
        acceptor.accept(*socket);
        std::cout << "[Connected]: " << socket->remote_endpoint().address().to_string() << ":" << socket->remote_endpoint().port() << std::endl;
        std::shared_ptr t = std::make_shared<std::thread>(session, socket);
        threads.emplace_back(t);
    } catch (const boost::system::system_error& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }
    for (auto& t : threads)
        t->join();
    return 0;
}
