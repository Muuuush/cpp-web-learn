#include "CimpleServer.hpp"
#include <iostream>

void recieveHello(std::shared_ptr<Session> session, uint16_t type, std::string_view message) {
    std::cout << "Receive: \"" << message << "\" from " << session->getRemoteEndpoint() << std::endl;
}

int main() {
    Server server(10086);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, recieveHello});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    return 0;
}
