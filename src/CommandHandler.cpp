#include "CommandHandler.hpp"
#include "User.hpp"

#include <arpa/inet.h>

#include <iostream>

// #include "utils.h"


constexpr unsigned int str2int(const char *str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

CommandHandler::CommandHandler(Server *server) {
//    this->user = user;
    this->server = server;
}

void CommandHandler::handleMessage(Message *message, User *user) {

    this->user = user;

    for (const parsedLine& parsed_line : message->getParsedMessage()) {
        handleCommand(parsed_line.command, parsed_line.text);
    }
}

void CommandHandler::handleCommand(const string& command, const string& text) {
    switch (str2int(command.c_str())) {
        case str2int("HELLO"):
            hello();
            break;

        case str2int("NICK"):
            nick(text);
            break;

        case str2int("HELP"):
            help();
            break;

        case str2int("WHO"):
            who(text);
            break;

        default:
            pass(text);
            break;
    }
}

void CommandHandler::hello() {
    sendResponse(HELLO_RESPONSE);
}

void CommandHandler::nick(const string &nickname) {
    string response;
    if (nickname.empty()) {
        response = "Invalid nickname";
    } else {
        user->setNickname(nickname);
        response = "Your nickname set to: " + nickname;
    }
    sendResponse(response);
}

void CommandHandler::help() {
    sendResponse(HELP_RESPONSE);
}

void CommandHandler::who(string text) {
    string response;
    if (text.empty()) {
        response = server->getConnectedToServerUserNicknames();
    } else {
        response = "server->getConnectedToChannelUserNickames(text)";
    }

    sendResponse(response);
}

void CommandHandler::sendResponse(const string &response) {
    int send_value = send(user->poll_fd.fd,
                          response.c_str(),
                          response.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while responding to user:" << user->getID() << endl;
    }
}

void CommandHandler::pass(const string& text) {
}

CommandHandler::~CommandHandler() = default;