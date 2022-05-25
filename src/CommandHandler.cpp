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
    defineAvailableCommands();
}

void CommandHandler::defineAvailableCommands() {
    available_commands.emplace_back("HELLO");
    available_commands.emplace_back("JOIN");
    available_commands.emplace_back("LEAVE");
    available_commands.emplace_back("QUIT");
    available_commands.emplace_back("HELP");
    available_commands.emplace_back("WHO");
    available_commands.emplace_back("LIST");
    available_commands.emplace_back("NICK");
}

void CommandHandler::handleMessage(Message *message, User *user) {

    this->user = user;

    for (parsedLine& parsed_line : message->getParsedMessage()) {
        handleCommand(parsed_line.command, parsed_line.text);
    }
}

void CommandHandler::handleCommand(const string& command, string& text) {
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

        case str2int("JOIN"):
            join(text);
            break;

        case str2int("LEAVE"):
            leave();
            break;

        case str2int("LIST"):
            list();
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
    response = HELLO_RESPONSE;
    sendResponse();
}

void CommandHandler::nick(const string &nickname) {

    if (nickname.empty()) {
        response = "Invalid nickname\n";
    } else {
        user->setNickname(nickname);
        response = "Your nickname set to: " + nickname + "\n";
    }

    sendResponse();
}

void CommandHandler::help() {
    response = HELP_RESPONSE;
    sendResponse();
}

void CommandHandler::join(string &channel_name) {


    if (channel_name.at(0) != '#') {
        channel_name = "#" + channel_name;
    }

    if (user->isConnectedToChannel()) {
        response = "You are already connected to channel " + user->getCurrentChannel() + "\n"
                   "To join other channel first LEAVE current channel\n";

    } else {
        if (server->doesChannelExists(channel_name)) {
            user->joinChannel(const_cast<string &>(channel_name));
            response = "Joined to channel " + channel_name + "\n";

        } else {
            response = "Channel " + channel_name + " does not exist\n";
        }
    }

    sendResponse();
}

void CommandHandler::leave() {

    if (user->isConnectedToChannel()) {
        string channel_name = user->getCurrentChannel();
        user->leaveChannel();
        response = "Leaved channel: " + channel_name + "\n";
    } else {
        response = "You have not joined any channel\n";
    }

    sendResponse();
}

void CommandHandler::list() {
    response = server->getChannelNames();

    sendResponse();
}

void CommandHandler::who(string channel_name) {

    if (!channel_name.empty()) {

        if (channel_name.at(0) != '#') {
            channel_name = "#" + channel_name;
        }

        if (server->doesChannelExists(channel_name)) {
            if (user->isConnectedToChannel()) {
                response = server->getConnectedToChannelUserNickames(user->getCurrentChannel());
            } else {
                response = server->getConnectedToChannelUserNickames(channel_name);
            }
        } else {
            response = "Channel " + channel_name + " does not exist\n";
        }

    } else {
        response = server->getConnectedToServerUserNicknames();
    }

    sendResponse();
}

void CommandHandler::sendResponse() {
    int send_value = send(user->poll_fd.fd,
                          response.c_str(),
                          response.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while responding to user:" << user->getID() << endl;
    }

    response = "";
}

void CommandHandler::pass(const string& text) {
}

bool CommandHandler::isValidCommand(string &command) {

    if (command.at(0) == '/') {
        command = command.substr(1);
    }

    for (const string& available_command : available_commands) {
        if (command == available_command) {
            return true;
        }
    }

    return false;
}

CommandHandler::~CommandHandler() = default;