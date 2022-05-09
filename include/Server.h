#ifndef SERVER_H
#define SERVER_H

#pragma once

class Server {
   public:
    Server(int);
    ~Server();

    // setters
    void setDomain(int);
    void setConnectionType(const char *);
    void setPort(int);

   private:
    int domain;
    int connectionType;
    int port;
    int socketDescriptor;

    const int PROTOCOL = 0;
    const int BACKLOG_QUEUE_SIZE = 1;

    void createSocket();
    void setSocketOptions();
    void bindSocket();
    void startListening();
};

#endif