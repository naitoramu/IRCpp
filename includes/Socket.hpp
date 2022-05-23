//
// Created by kuba on 5/23/22.
//

#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

class Socket
{
public:
    Socket(int);
    ~Socket();

    void startListening();

    int getFileDescriptor();
    short getAddressFamily();
    in_addr *getInAddr();
    sockaddr_in *getSockAddrIn();

private:
    int *socket_fd;
    sockaddr_in *socket_address;

    const int PORT;
    const short ADDRESS_FAMILY = AF_INET;
    const int TCP = SOCK_STREAM;
    const int PROTOCOL = 0;
    const int ALLOWED_IP = INADDR_ANY;
    const int BACKLOG_QUEUE_SIZE = 32;

    void create();
    void setOptions();
    void bindSc();

    int validatePortNumber(int);

};
