#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

#include "Channel.h"

#pragma once

using namespace std;

class User {
   public:
    User(string, string, vector<unsigned int> *);
    User();
    ~User();

    // setters
    void setUsername(string);
    void setPassword(string);
    void setChannels(vector<unsigned int> *);

    // getters

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    vector<unsigned int> * getChannels() {
        return channels;
    }

   private:
    int socket_fd;
    string password;
    string username;
    string hostname;

    vector<unsigned int> *channels;
};

#endif