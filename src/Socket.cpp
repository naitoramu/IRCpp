//
// Created by kuba on 5/23/22.
//

#include "Socket.hpp"
#include <fcntl.h>
#include <string.h>

#include <iostream>

using namespace std;

Socket::Socket(int port) : PORT(validatePortNumber(port)) {

    socket_fd = new int;
    socket_address = new sockaddr_in;

    create();
    setOptions();
    bindSc();
}

int Socket::validatePortNumber(int port) {
    if (port < 1024 || port > 49151) {
        cerr << "Invalid port number: " << port << endl;
        cerr << "Available range: 1024 - 49151" << endl;
        exit(EXIT_FAILURE);

    } else {
        return port;
    }
}

void Socket::create() {
    *socket_fd = socket(ADDRESS_FAMILY, TCP, PROTOCOL);

    if (*socket_fd == -1) {
        cerr << "Failed to create socket. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::setOptions() {
    const int trueFlag = 1;

    int status = setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));

    if (status < 0) {
        cerr << "Setsockopt failed. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::bindSc() {
    socket_address->sin_family = ADDRESS_FAMILY;
    socket_address->sin_addr.s_addr = ALLOWED_IP;
    socket_address->sin_port = htons(PORT);

    int status = bind(*socket_fd, (struct sockaddr*)socket_address, sizeof(*socket_address));

    if (status < 0) {
        cerr << "Failed to bind to port '" << PORT << "'. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::startListening() {
    int status;
    status = listen(*socket_fd, BACKLOG_QUEUE_SIZE);

    if (status < 0) {
        cerr << "Failed to listen on socket. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    fcntl(*socket_fd, F_SETFL, O_NONBLOCK);  // setting socket as nonblocking
}

// getters

int Socket::getFileDescriptor() {
    return *socket_fd;
}

short Socket::getAddressFamily() {
    return ADDRESS_FAMILY;
}

in_addr *Socket::getInAddr() {
    return &(socket_address->sin_addr);
}

sockaddr_in *Socket::getSockAddrIn() {
    return socket_address;
}

Socket::~Socket() {
    delete socket_fd;
    delete socket_address;
}