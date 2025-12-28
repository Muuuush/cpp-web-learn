# CimpleServer
This is a simple but high performance framework of a TCP C++ server. Only to offer some callback functions, you can setup a simple server very quickly.

## Quick Start
You need to seed port when initialize the server and set callback functions map before it get started.
When a new packet arrives the server, logic system will call the corresponding callback function according to its type.

`LogicSystem::CallbackFunction` accepted 3 parameters
1. `std::shared_ptr<Session> session`
    It contains the information of the client and the session.
    You can get them by these functions:
    - `getUUID()` get the UUID of the session.
    - `getRemoteEndpoint()` get the client endpoint of the session.
    - `toString()` get formatted string looks like "address:port [UUID]".
2. `uint16_t type`
    It is the type of the message. It should follow the protocol between server and client.
3. `std::string message`
    The message content. You can get it by constant reference to avoid copy.

A simple echo server example.
``` cpp
#include "CimpleServer.hpp"
#include <iostream>

void echo(std::shared_ptr<Session> session, uint16_t type, const std::string& message) {
    std::cout << "Receive: \"" << message << "\" from " << session->getRemoteEndpoint() << std::endl;
    session->send(SendNode(type, message));
}

int main() {
    Server server(10086);
    std::unordered_map<uint16_t, LogicSystem::CallbackFunction> callbacks;
    callbacks.insert({0, echo});
    LogicSystem::setCallbacks(callbacks);
    server.start();
    return 0;
}
```

## Packet
This framework uses TLV (Type-Lenth-Value) format packet.

|  Type   | Length  |      Value       |
|:-------:|:-------:|:----------------:|
| 2 Bytes | 2 Bytes | Max 65,531 Bytes |

The `type` of the packet will determine the callback function to be called in the server, via the hash map.

## Features
- High-performance network I/O
- Decoupled logic layer and network I/O
- Supporting multi-threaded logic processing (only one thread by default, you can changed it by passing the `ServerSetting` parameter when initialize the `Server`)
- Dynamic update callback functions (This will pause the logic process temporally, please pay attention)

## Setup Setting
You can change the setting of server via `ServerSetting`
``` cpp
struct ServerSetting {
    int backlog = boost::asio::socket_base::max_listen_connections;
    int ioContextPoolSize = std::thread::hardware_concurrency();
    int logicQueueCapacity = 1024;
    int logicWorkerNum = 1;
    ServerSetting() = default;
};
// Then set up the server like:
ServerSetting s;
s.logicWorkerNum = 114514;
Server server(10086. s);
```

## How to build client
In the this framework, we provide a simple Client class to build a simple client. You can check the example's for reference.

However, all the network IO are synchronous, you can build customized client by any method you like, as long as you follow the TLV format packet we mentioned [before](##Packet).

## Examples
- [Echo server and client](./example/echo)
- [Hello server and client](./example/hello)
- [Server stress testing and example of multi-threaded logic processing server](./example/many-greeting)

## Dependency
- `Boost.Asio` (Boost Software License 1.0)
- `spdlog` (MIT License)
