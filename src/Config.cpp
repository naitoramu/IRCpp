#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GlobalVariables.h"
#include "User.h"

template <class T>
class Config {
   private:
    string filename;
    fstream file;

    vector<T> *objects;

   public:
    Config(string filename) {
        this->filename = filename;
    }

    ~Config() {
    }

   private:
    void writeUsers() {
        vector<User> users(objects->begin(), objects->end());

        for (User user : users) {
            file << user.getUsername() << ',' << user.getPassword();

            for (Channel channel : *(user.getChannels())) {
                file << ',' << channel.getID();
            }

            file << '\n';
        }
    }

   public:
    void write(vector<T> *objects) {
        file.open(filename, ios::out | ios::app);
        this->objects = objects;

        if (is_same<T, User>::value) {
            writeUsers();

            if (DEBUG_MODE) {
                cout << "Config::writeUsers();" << endl;
            }
        } else {
            cout << "Different" << endl;
        }
    }
};

#endif