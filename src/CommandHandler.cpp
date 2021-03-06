#include "CommandHandler.hpp"
#include "User.hpp"

#include <arpa/inet.h>

#include <iostream>
#include <cstring>

// function necessary for switch(){}
constexpr unsigned int str2int(const char *str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

CommandHandler::CommandHandler(Server *server) {
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
    available_commands.emplace_back("USER");
}

void CommandHandler::handleMessage(Message *message, User *user) {

    this->user = user;

    for (parsedLine &parsed_line: message->getParsedMessage()) {
        handleCommand(parsed_line.command, parsed_line.text);
    }
}

void CommandHandler::handleCommand(const string &command, string &text) {
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

        case str2int("QUIT"):
            quit();
            break;

        default:
            pass(text);
            break;
    }
}

void CommandHandler::sendResponse() {

    response = SPLITTER + response + SPLITTER;

    int send_value = send(user->poll_fd.fd,
                          response.c_str(),
                          response.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while sending response:" << strerror(errno) << endl;
    }

    response.clear();
}

void CommandHandler::hello() {
    response = HELLO_RESPONSE;
    sendResponse();
}

void CommandHandler::help() {
    response = HELP_RESPONSE;
    sendResponse();
}

void CommandHandler::list() {
    response = server->getChannelNames();

    sendResponse();
}

void CommandHandler::quit() {
    response = "SEE YOU\n";

    sendResponse();

    server->removeDissconnectedClient(user->poll_fd.fd);
}

void CommandHandler::nick(const string &nickname) {

    if (nickname.empty()) {
        if (user->getNickname().empty()) {
            response = "Invalid nickname\n";

        } else {
            response = "Your nickname: " + user->getNickname() + "\n";

        }
    } else {
        if (user->getNickname().empty()) {
            user->setNickname(nickname);

        } else {
            response = "Nickname already set\n";

        }
        response = response + "Your nickname: " + user->getNickname() + "\n";

    }

    sendResponse();
}

void CommandHandler::join(string &channel_name) {


    if (channel_name.at(0) != '#') {
        channel_name = "#" + channel_name;
    }

    if (user->isConnectedToChannel()) {
        response = "You are already connected to channel " + user->getCurrentChannelName() + "\n"
                                                                                             "To join other channel first LEAVE current channel\n";

    } else {
        if (server->channelExists(channel_name)) {
            user->joinChannel(const_cast<string &>(channel_name));
            response = "Joined channel: " + channel_name + "\n";
            message = SPLITTER + user->getNickname() + " joined the channel\n" + SPLITTER;
            forwardMessageToUsersOnSameChannel(channel_name);

        } else {
            response = "Channel " + channel_name + " does not exist\n";

        }
    }

    sendResponse();
}

void CommandHandler::leave() {

    if (user->isConnectedToChannel()) {
        string channel_name = user->getCurrentChannelName();
        user->leaveChannel();
        response = "Channel " + channel_name + " left\n";
        message = SPLITTER + user->getNickname() + " left the channel\n" + SPLITTER;
        forwardMessageToUsersOnSameChannel(channel_name);

    } else {
        response = "You have not joined any channel\n";

    }

    sendResponse();
}

void CommandHandler::who(string channel_name) {

    if (!channel_name.empty()) {

        if (channel_name.at(0) != '#') {
            channel_name = "#" + channel_name;
        }

        if (server->channelExists(channel_name)) {
            response = server->getConnectedToChannelUserNickames(channel_name);

        } else {
            response = "Channel " + channel_name + " does not exist\n";

        }

    } else {
        if (user->isConnectedToChannel()) {
            response = server->getConnectedToChannelUserNickames(user->getCurrentChannelName());

        } else {
            response = server->getConnectedToServerUserNicknames();

        }
    }

    sendResponse();
}

void CommandHandler::pass(const string &text) {
    if (user->isConnectedToChannel()) {
        message = user->getNickname() + ": " + text;
        forwardMessageToUsersOnSameChannel(user->getCurrentChannelName());

    } else {
        response = "Invalid command\n"
                   "To send messages you need to join channel\n"
                   "To get some help type 'HELP'\n";

        sendResponse();
    }
}

void CommandHandler::forwardMessageToUsersOnSameChannel(const string &channel_name) {

    vector<int> *file_descriptors = server->getUserOnChannelFileDescriptors(channel_name);

    for (int file_descriptor: *file_descriptors) {
        if (file_descriptor != user->poll_fd.fd) {
            cout << "Forwarding to fd: " << file_descriptor << endl;
            sendMessage(file_descriptor);
        }
    }

    message.clear();
    delete file_descriptors;
}

void CommandHandler::sendMessage(int &file_descriptor) {
    int send_value = send(file_descriptor,
                          message.c_str(),
                          message.length(),
                          0);
    if (send_value < 0) {
        cerr << "Error while sending message:" << strerror(errno) << endl;
    }

}

bool CommandHandler::isValidCommand(string &command) {

    if (command.at(0) == '/') {
        command = command.substr(1);
    }

    for (const string &available_command: available_commands) {
        if (command == available_command) {
            return true;
        }
    }

    return false;
}

// destructor
CommandHandler::~CommandHandler() = default;