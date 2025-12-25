#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

const std::string SERVER_IP = "127.0.0.1";
const int PORT = 10086;
const int MAX_LENGTH = 1024;

int main() {
    try {
        std::cout << ">> ";
        std::string input;
        std::getline(std::cin, input);
        io_context ioContext;
        ip::tcp::endpoint serverEP(ip::make_address(SERVER_IP), PORT);
        ip::tcp::socket socket(ioContext, serverEP.protocol());
        socket.connect(serverEP);
        std::cout << "Connect success to " << serverEP.address().to_string() << ": " << serverEP.port() << std::endl;
        write(socket, buffer(input));
        char reply[MAX_LENGTH] = {};
        read(socket, buffer(reply, input.size()));
        std::cout << "[Receive]: " << reply << std::endl;
        socket.close();
    } catch (const boost::system::system_error& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }
    return 0;
}
