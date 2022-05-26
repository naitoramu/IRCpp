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
    static unsigned int id_counter;

private:
    const unsigned int ID;
    string nickname;
    string current_channel_name;
    bool connected_to_channel = false;

public:
    explicit User(pollfd);

    ~User();

    void setNickname(string);

    void joinChannel(string &);

    void leaveChannel();

    bool isConnectedToChannel();

    string getNickname();

    const string &getCurrentChannelName() const;

};