#pragma once

#include <sys/poll.h>

#include <memory>
#include <string>

#include "Message.hpp"

using namespace std;

class CommandHandler;

class User {
public:
    User(User const &user);

    pollfd poll_fd{};

    // User(const User &);
    User(pollfd);
    ~User();

    pollfd getPollFD() const;
    unsigned int getID() const;

    void handleMessage(Message *);
    void handleCommand(string, string);

    static unsigned int id_counter;

private:
    const unsigned int ID;

    string nickname;
    bool connected_to_channel{};
    string joined_channel_id;

    unique_ptr<CommandHandler> command_handler;

    // const string USERNAME;
};