#include "CimpleServer.hpp"
#include <iostream>

std::atomic_int helloCnt;
std::atomic_int hiCnt;
void recieveHello(std::shared_ptr<Session> session, uint16_t type, std::string_view message) {
    helloCnt++;
}
void recieveHi(std::shared_ptr<Session> session, uint16_t type, std::string_view message) {
    hiCnt++;
}

int main() {
    ServerSetting setting;
    setting.logicWorkerNum = 4;
    Server server(10086, setting);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, recieveHello});
    callbacks.insert({1, recieveHi});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    std::cout << "Total received hello count: " << helloCnt << std::endl;
    std::cout << "Total received hi count: " << hiCnt << std::endl;
    return 0;
}
