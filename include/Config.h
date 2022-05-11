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

    vector<T> read();
    void write(vector<T> *);

   private:
    string filename;
    fstream file;

    vector<T> *objects;

    void writeUsers();
};

#endif