#include "PacketNode.hpp"
#include "Client.hpp"
#include <boost/asio.hpp>

using namespace boost::asio;
int main() {
    Client client;
    ip::tcp::endpoint serverEP(ip::make_address("127.0.0.1"), 10086);
    boost::system::error_code ec;
    client.connect(serverEP, ec);
    TLVPacket node(0, "Hello server!");
    client.sendPacket(node);
    return 0;
}
