#include "CimpleServer.hpp"
#include <iostream>

void echo(std::shared_ptr<Session> session, uint16_t tag, std::string_view message) {
    std::cout << "Receive: \"" << message << "\" from " << session->getRemoteEndpoint() << std::endl;
    std::optional<boost::system::system_error> result = session->send(TLVPacket(tag, message));
    if (result.has_value()) {
        std::cout << "Echo failed to " << session->getRemoteEndpoint() << std::endl;
    }
}

int main() {
    Server server(10086);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, echo});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    return 0;
}
