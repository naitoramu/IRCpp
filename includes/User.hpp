#pragma once

#include <sys/poll.h>

#include <memory>
#include <string>

#include "Message.hpp"
#include "Server.hpp"

using namespace std;

class Message;
class CommandHandler;

class User {
public:
    pollfd poll_fd{};

    explicit User(pollfd);
    ~User();

    unsigned int getID() const;
    string getNickname();

    void setNickname(string);
    void joinChannel(string &);
    void leaveChannel();

    bool isConnectedToChannel();

    static unsigned int id_counter;

private:
    const unsigned int ID;

    string nickname = "";
    bool connected_to_channel = false;
    string current_channel = "NULL";
public:
    const string &getCurrentChannel() const;

};