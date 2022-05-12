#ifndef CONFIG_CPP
#define CONFIG_CPP

#include "Config.h"

#include <iostream>
#include <sstream>

#include "Channel.h"
#include "GlobalVariables.h"
#include "Server.h"
#include "User.h"

template <class T>
Config<T>::Config(string filename) {
    this->filename = "config/" + filename;

    if (is_same<T, User>::value) {
        class_type = USER;
    } else if (is_same<T, Server>::value) {
        class_type = SERVER;
    } else if (is_same<T, Channel>::value) {
        class_type = CHANNEL;
    } else {
        cerr << "Invalid class T" << endl;
        exit(EXIT_FAILURE);
    }
}

template <class T>
Config<T>::~Config() {
}

template <class T>
void Config<T>::write(vector<T> *objects) {
    file.open(filename, ios::out | ios::app);
    this->objects = objects;

    switch (class_type) {
        case USER:
            writeUsers();
            break;
        case SERVER:
            // writeUsers();
            break;
        case CHANNEL:
            // writeUsers();
            break;
        default:
            cerr << "Invalid class_type" << endl;
            exit(EXIT_FAILURE);
            break;
    }
}

template <class T>
void Config<T>::writeUsers() {
    for (User user : *objects) {
        file << user.getUsername() << ',' << user.getPassword();

        for (unsigned int channel : *(user.getChannels())) {
            file << ',' << channel;
        }

        file << '\n';
    }
}

template <class T>
vector<T> *Config<T>::read() {
    file.open(filename, ios::in);
    objects = new vector<T>();

    switch (class_type) {
        case USER:
            readUsers();
            break;
        case SERVER:
            // writeUsers();
            break;
        case CHANNEL:
            // writeUsers();
            break;
        default:
            cerr << "Invalid class_type" << endl;
            exit(EXIT_FAILURE);
            break;
    }

    return objects;
}

template <class T>
void Config<T>::readUsers() {
    
    string row, cell;

    while(getline(file, row)) {

        stringstream str(row);
        int counter = 0;
        vector<unsigned int> * channels = new vector<unsigned int>();
        User user;

        while(getline(str, cell, ',')) {

            if (counter == 0) {
                user.setUsername(cell);
            } else if (counter == 1) {
                user.setPassword(cell);
            } else {
                channels->push_back(stoi(cell));
            }

            counter++;
        }

        user.setChannels(channels);
        objects->push_back(user);

    }

}

#endif