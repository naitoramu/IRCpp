#pragma once

#include <sys/poll.h>

#include <string>
#include <vector>

#include "Socket.hpp"
#include "User.hpp"

using namespace std;

class Server {
public:
    Server(int);
    ~Server();

    void grabConnection();
    void handleClients();

    static vector<string> available_commands;
    static bool isValidCommand(string);

private:
    Socket *socket;
    vector<pollfd> connected_fds;
    vector<unique_ptr<User>> connected_users;

    const int IP_STRING_LENGTH = INET_ADDRSTRLEN;
    const id_t INACTIVITY_TIMEOUT = 10;

    User * getUserByFD(int);
};