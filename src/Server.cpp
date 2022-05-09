#include "Server.h"

#include <netinet/in.h>
#include <string.h>

#include <iostream>

#include "GlobalVariables.h"
#include "stdlib.h"
#include "sys/socket.h"

using namespace std;

Server::Server(int port) {
    setDomain(AF_INET);
    setConnectionType("TCP");
    setPort(port);

    createSocket();
    setSocketOptions();
    bindSocket();
    startListening();
}

Server::~Server() {
}

void Server::createSocket() {
    if (domain && connectionType && port) {
        socketDescriptor = socket(domain, connectionType, PROTOCOL);

        if (socketDescriptor == -1) {
            cerr << "Failed to create socket. Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }

        if (DEBUG_MODE) {
            cout << "socket(" << domain << ", " << connectionType << ", " << PROTOCOL << ");" << endl;
        }
    } else {
        cerr << "NULL pointer: domain or connectionType or port is NULL" << endl;
    }
}

void Server::setSocketOptions() {
    const int trueFlag = 1;

    int status = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));

    if (status < 0) {
        cerr << "Setsockopt failed. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket() {
    struct sockaddr_in address;

    address.sin_family = domain;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int status = bind(socketDescriptor, (struct sockaddr*)&address, sizeof(address));

    if (status < 0) {
        cerr << "Failed to bind to port '" << port << "'. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::startListening() {
    int listenStatus;
    listenStatus = listen(socketDescriptor, BACKLOG_QUEUE_SIZE);

    if (listenStatus < 0) {
        cerr << "Failed to listen on socket. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // fcntl(socketDescriptor, F_SETFL, O_NONBLOCK);
}

// setters

void Server::setDomain(int domain) {
    this->domain = domain;
}

void Server::setPort(int port) {
    if (port < 1024 || port > 49151) {
        cerr << "Invalid port number: " << port << endl;
        cerr << "Available range: 1024 - 49151" << endl;
        exit(EXIT_FAILURE);

    } else {
        this->port = port;
    }
}

void Server::setConnectionType(const char* connectionType) {
    if (!strcmp(connectionType, "TCP")) {
        this->connectionType = SOCK_STREAM;
        if (DEBUG_MODE) {
            cout << "TCP selected" << endl;
        }

    } else if (!strcmp(connectionType, "UDP")) {
        this->connectionType = SOCK_DGRAM;
        if (DEBUG_MODE) {
            cout << "UDP selected" << endl;
        }

    } else {
        cerr << "Invalid type for connection: " << connectionType << endl;
        exit(EXIT_FAILURE);
    }
}