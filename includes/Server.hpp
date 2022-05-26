#pragma once

#include <sys/poll.h>

#include <string>
#include <vector>

#include "Socket.hpp"
#include "User.hpp"

using namespace std;

class User;

class Message;

class CommandHandler;

class Server {

private:

    const int IP_STRING_LENGTH = INET_ADDRSTRLEN;
    const id_t INACTIVITY_TIMEOUT = 10;

    char messageBuffer[1024];
    Socket *socket;
    Message *message;
    CommandHandler *command_handler;
    vector<pollfd> connected_fds;
    vector<unique_ptr<User>> connected_users;
    vector<string> channels;

public:

    Server(int);

    ~Server();

    void grabConnection();

    void handleClients();

    void removeDissconnectedClient(int &);

    bool channelExists(const string &);

    string getConnectedToChannelUserNickames(const string &);

    string getConnectedToServerUserNicknames();

    string getChannelNames();

    vector<int> *getUserOnChannelFileDescriptors(const string &);

private:

    void defineChannels();

    void handleWaitingClients();

    void removeDisconnectedFileDescriptor(int &fd);

    void removeDisconnectedUser(int &fd);

    User *getUserByFileDescriptor(int);

};