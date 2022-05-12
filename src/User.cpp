#include "User.h"

User::User(string username, string password, vector<unsigned int> *channels) {
    this->username = username;
    this->password = password;

    this->channels = channels;
}

User::User() {
}

User::~User() {
}

// setters
void User::setUsername(string username) {
    this->username = username;
}

void User::setPassword(string password) {
    this->password = password;
}

void User::setChannels(vector<unsigned int> *channels) {
    this->channels = channels;
}