#include "PacketNode.hpp"
#include "Client.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio;
std::atomic_int successHello;
std::atomic_int successHi;
void sendGreeting(int greetingCnt) {
    try {
        Client client;
        ip::tcp::endpoint serverEP(ip::make_address("127.0.0.1"), 10086);
        boost::system::error_code ec;
        do
            client.connect(serverEP, ec);
        while (ec.failed());
        TLVPacket hello(0, "Hello server!");
        TLVPacket hi(1, "Hi server!");
        while (greetingCnt--) {
            if (greetingCnt % 2 == 0) {
                client.sendPacket(hello);
                successHello++;
            } else {
                client.sendPacket(hi);
                successHi++;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[Error]: " << e.what() << std::endl;
        return;
    }
}
int main() {
    const int greetingPerThread = 5000;
    const int threadCnt = 200;
    std::vector<std::thread> threads;
    threads.reserve(threadCnt);
    for (int i = 0; i < threadCnt; i++)
        threads.emplace_back(sendGreeting, greetingPerThread);
    for (auto& t : threads) t.join();
    std::cout << "total " << successHello << " hello messages was sent." << std::endl;
    std::cout << "total " << successHi << " hi messages was sent." << std::endl;
    return 0;
}
