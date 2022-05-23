#include "CommandHandler.hpp"
#include "User.hpp"

#include <arpa/inet.h>

#include <iostream>

// #include "utils.h"


constexpr unsigned int str2int(const char *str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

CommandHandler::CommandHandler(User *user) {
    this->user = user;
}

void CommandHandler::handle(const string& command, const string& text) {
    switch (str2int(command.c_str())) {
        case str2int("HELLO"):
            hello();
            break;

        case str2int("HELP"):
            help();
            break;

        default:
            pass(text);
            break;
    }
}

void CommandHandler::hello() {
    int send_value = send(user->poll_fd.fd,
                          HELLO_RESPONSE.c_str(),
                          HELLO_RESPONSE.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while responding to user:" << user->getID() << endl;
    }
}

void CommandHandler::help() {
    int send_value = send(user->poll_fd.fd,
                          HELP_RESPONSE.c_str(),
                          HELP_RESPONSE.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while responding to user:" << user->getID() << endl;
    }
}

void CommandHandler::pass(const string& text) {
}

CommandHandler::~CommandHandler() = default;