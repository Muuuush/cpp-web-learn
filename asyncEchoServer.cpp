#include <iostream>
#include <boost/asio.hpp>

#include "Server.hpp"

using namespace boost::asio;

int main() {
    try {
        io_context ioc;
        Server server(ioc, 10086);
        server.start();
        ioc.run();
    } catch (const boost::system::system_error& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
    }
    return 0;
}
