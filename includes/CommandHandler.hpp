#pragma once

#include <string>
#include "Server.hpp"

using namespace std;

class User;
class Server;

class CommandHandler {
public:
    CommandHandler(Server *);

    ~CommandHandler();

    void handleMessage(Message *, User *);

private:
    User *user;
    Server *server;

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

    void handleCommand(const string&, const string&);
    void sendResponse(const string&);

    void hello();
    void nick(const string &);
    void pass(const string &);
    void who(string);
    void help();

};