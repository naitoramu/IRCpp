#include "User.hpp"

#include <iostream>

using namespace std;

User::User(pollfd poll_fd) : ID(id_counter++) {
    cout << "User(pollfd poll_fd) used" << endl;
    this->poll_fd = poll_fd;
}

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

void User::joinChannel(string &channel_name) {
    current_channel = channel_name;
    connected_to_channel = true;
}

void User::leaveChannel() {
    current_channel = "NULL";
    connected_to_channel = false;
}

bool User::isConnectedToChannel() {
    return connected_to_channel;
}

const string &User::getCurrentChannel() const {
    return current_channel;
}

User::~User() = default;
