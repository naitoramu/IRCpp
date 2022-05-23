#pragma once

#include <string>

using namespace std;

class User;

class CommandHandler {
public:
    CommandHandler(User *);
    ~CommandHandler();

    void handle(const string&, const string&);

private:
    User *user;

    const string HELLO_RESPONSE = "Welcome on IRCpp server\n"
                                  "To start chatting join a channel\n"
                                  "To get some help type 'HELP'\n";
    const string HELP_RESPONSE = "Available commands:\n\n"
                                 "HELP                      display this help\n"
                                 "JOIN <#channel>           join to channel\n"
                                 "LIST                      list available channels\n"
                                 "LEAVE                     leave current channel\n"
                                 "WHO                       list online users on server\n"
                                 "WHO <#channel>            list online users on chosen channel\n"
                                 "QUIT                      quit from server\n";
    void hello();
    void pass(const string&);

    void help();
};