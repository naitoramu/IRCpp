#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <vector>

#pragma once

using namespace std;

template <class T>
class Config {
   public:
    Config(string);
    ~Config();

    vector<T> * read();
    void write(vector<T> *);

   private:
    string filename;
    fstream file;
    vector<T> *objects;

    enum ClassType { USER, SERVER, CHANNEL };
    ClassType class_type;

    void writeUsers();
    void readUsers();
};

#include "../src/Config.cpp"

#endif