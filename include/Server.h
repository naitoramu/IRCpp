#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <vector>

#include "User.h"

#pragma once
class Server {
   public:
    Server(int, int);  // : IP_VERSION(), IP_STRING_LENGTH(){};
    ~Server();

    // setters
    void setConnectionType(const char *);
    void setPort(int);
    int getIpStringLength(int);
    void handleClients();

   private:
    int connection_type;
    int port;
    int socket_fd;
    struct sockaddr_in socket_address;

    vector<User> *users;

    const int IP_VERSION;
    const int IP_STRING_LENGTH;
    const int ADDRESS_FAMILY = AF_INET;
    const int PROTOCOL = 0;
    const int ALLOWED_IP = INADDR_ANY;
    const int BACKLOG_QUEUE_SIZE = 1;

    void createSocket();
    void setSocketOptions();
    void bindSocket();
    void startListening();
    void grabConnection();

    void setIpVersion(int);
    void setIpStringLength(int);
};

#endif