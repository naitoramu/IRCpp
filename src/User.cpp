#include "User.h"

User::User(string username, string password, vector<Channel> *channels) {
    this->username = username;
    this->password = password;

    this->channels = channels;
}

User::~User() {
}