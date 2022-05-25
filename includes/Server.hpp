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
public:
    Server(int);
    ~Server();

    void grabConnection();
    void handleClients();

    string getConnectedToServerUserNicknames();

    static vector<string> available_commands;

    bool doesChannelExists(const string &);

    string getConnectedToChannelUserNickames(const string &);

    string getChannelNames();

private:
    Socket *socket;
    Message *message;
    CommandHandler *command_handler;
    vector<pollfd> connected_fds;
    vector<unique_ptr<User>> connected_users;
    vector<string> channels;

    const int IP_STRING_LENGTH = INET_ADDRSTRLEN;
    const id_t INACTIVITY_TIMEOUT = 10;

    User * getUserByFD(int);

    void removeDissconnectedClient(int &);

    void removeDisconnectedFileDescriptor(int &fd);

    void removeDisconnectedUser(int &fd);

    void defineChannels();
};