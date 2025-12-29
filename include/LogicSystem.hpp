#pragma once

#include "Singleton.hpp"
#include "Session.hpp"
#include "LogicNode.hpp"
#include <functional>
#include <shared_mutex>
#include <unordered_map>

class LogicSystem : public Singleton<LogicSystem> {
    friend Singleton<LogicSystem>;
public:
    using CallbackFunction = std::function<void(std::shared_ptr<Session> session, uint16_t type, std::string_view message)>;
private:
    LogicSystem(int capacity, int workerNum);

    void worker();
public:
    void registerNode(LogicNode&& node);
    static std::unordered_map<uint16_t, CallbackFunction> getCallbacks() noexcept;
    static void setCallbacks(std::unordered_map<uint16_t, CallbackFunction> value) noexcept;

    ~LogicSystem();

private:
    inline static std::unordered_map<uint16_t, CallbackFunction> callbacks;
    inline static std::shared_mutex callbacksMutex;
    std::vector<LogicNode> logicQueue;
    int capacity;
    int head;
    int tail;
    int fill;
    std::vector<std::thread> workers;
    std::mutex queueMutex;
    std::condition_variable empty;
    std::condition_variable full;
    bool stop;
};

inline std::unordered_map<uint16_t, LogicSystem::CallbackFunction> LogicSystem::getCallbacks() noexcept {
    std::shared_lock lock(callbacksMutex);
    return callbacks;
}

inline void LogicSystem::setCallbacks(std::unordered_map<uint16_t, CallbackFunction> value) noexcept {
    std::unique_lock lock(callbacksMutex);
    callbacks = value;
}
