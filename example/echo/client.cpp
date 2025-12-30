#include "PacketNode.hpp"
#include "Client.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
int main() {
    Client client;
    ip::tcp::endpoint serverEP(ip::make_address("127.0.0.1"), 10086);
    boost::system::error_code ec;
    client.connect(serverEP, ec);
    std::string input;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        TLVPacket node(0, input);
        client.sendPacket(node);
        auto recieve = client.recievePacket();
        std::cout << "echo: " << recieve.getMessage() << std::endl;
    }
    return 0;
}
