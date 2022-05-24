//
// Created by kuba on 5/23/22.
//

#include "User.hpp"
#include "CommandHandler.hpp"

#include <iostream>

using namespace std;

// User::User(const User &) : ID(0) {}
//[[maybe_unused]] User::User(User const &user) : ID(0) {
//    cout << "User(User const &user) used" << endl;
//}

User::User(pollfd poll_fd) : ID(id_counter++) {
    cout << "User(pollfd poll_fd) used" << endl;
    this->poll_fd = poll_fd;
//    this->command_handler = make_unique<CommandHandler>(this, server);
}

//void User::handleMessage(Message *message) {
//    for (const parsedLine& parsed_line : message->getParsedMessage()) {
//        command_handler->handle(parsed_line.command, parsed_line.text);
//    }
//}

// setters
void User::setNickname(string nickname) {
    this->nickname = nickname;
}

// getters
string User::getNickname() {
    return nickname;
}

unsigned int User::getID() const {
    return ID;
}

User::~User() = default;
