#include "CimpleServer.hpp"
#include <iostream>
#include <format>

std::atomic_int helloCnt;
std::atomic_int hiCnt;
void recieveHello(std::shared_ptr<Session> session, uint16_t tag, std::string_view message) {
    helloCnt++;
}
void recieveHi(std::shared_ptr<Session> session, uint16_t tag, std::string_view message) {
    hiCnt++;
}

void ShowReceive(std::stop_token stop) {
    while (!stop.stop_requested()) {
        if (helloCnt != 0 || hiCnt != 0)
            std::cout << std::format("\033[2K\rRecieved hello: {}, hi: {}", helloCnt.load(), hiCnt.load()) << std::flush;
        std::this_thread::sleep_for(std::chrono::microseconds(250));
    }
    std::cout << std::endl;
}

int main() {
    auto t = std::jthread(ShowReceive);

    ServerSetting setting;
    setting.logicWorkerNum = 4;
    setting.stopCallback = [&t] { t.request_stop(); };
    Server server(10086, setting);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, recieveHello});
    callbacks.insert({1, recieveHi});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    return 0;
}
