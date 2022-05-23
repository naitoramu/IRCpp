cmake_minimum_required(VERSION 3.23)
project(IRCpp)

set(CMAKE_CXX_STANDARD 23)

include_directories(includes)

add_executable(IRCpp src/main.cpp src/Socket.cpp includes/Socket.hpp src/User.cpp includes/User.hpp src/Message.cpp includes/Message.hpp src/Server.cpp includes/Server.hpp src/CommandHandler.cpp includes/CommandHandler.hpp)
