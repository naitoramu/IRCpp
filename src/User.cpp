#include "User.hpp"

#include <iostream>

using namespace std;

User::User(pollfd poll_fd) : ID(id_counter++) {
    this->poll_fd = poll_fd;
}

void User::joinChannel(string &channel_name) {
    current_channel_name = channel_name;
    connected_to_channel = true;
}

void User::leaveChannel() {
    current_channel_name = "NULL";
    connected_to_channel = false;
}

bool User::isConnectedToChannel() {
    return connected_to_channel;
}


// getters
string User::getNickname() {
    return nickname;
}

const string &User::getCurrentChannelName() const {
    return current_channel_name;
}

// setters
void User::setNickname(string nickname) {
    this->nickname = nickname;
}

// destructor
User::~User() = default;
