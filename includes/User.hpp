#pragma once

#include <sys/poll.h>

#include <memory>
#include <string>

#include "Message.hpp"
#include "Server.hpp"

using namespace std;

class CommandHandler;

class User {
public:
    pollfd poll_fd{};

    User(pollfd);
    ~User();

    unsigned int getID() const;
    string getNickname();

    void setNickname(string);

    void handleMessage(Message *);
    void handleCommand(string, string);

    static unsigned int id_counter;

private:
    const unsigned int ID;

    string nickname = "";
    bool connected_to_channel{};
    string joined_channel_id;

//    unique_ptr<CommandHandler> command_handler;

    // const string USERNAME;
};