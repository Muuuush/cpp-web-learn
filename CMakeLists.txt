cmake_minimum_required(VERSION 4.0)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cpp-web-learn)
find_package(Boost 1.89 REQUIRED)

add_executable(main ./main.cpp)

add_executable(basicSocket ./basicSocket.cpp)

add_executable(basicEchoServer ./basicEchoServer.cpp)
add_executable(basicEchoClient ./basicEchoClient.cpp)

add_executable(asyncEchoServer ./asyncEchoServer.cpp ./Server.cpp ./Session.cpp)

target_link_libraries(main ${Boost_LIBARIES})
