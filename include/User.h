#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

#include "Channel.h"

#pragma once

using namespace std;

class User {
   public:
    User(string, string, vector<Channel> *);
    ~User();

    // getters

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    vector<Channel> *getChannels() {
        return channels;
    }

   private:
    int socket_fd;
    string password;
    string username;
    string hostname;

    vector<Channel> *channels;
};

#endif