#include "Server.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>

#include <iostream>

#include "GlobalVariables.h"
#include "Config.h"
#include "stdlib.h"

using namespace std;

Server::Server(int ip_version, int port) : IP_VERSION(ip_version),
                                           IP_STRING_LENGTH(getIpStringLength(ip_version)) {
    Config<User> usersData("users.csv");
    users = usersData.read();

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
    if (ADDRESS_FAMILY && connection_type && port) {
        socket_fd = socket(ADDRESS_FAMILY, connection_type, PROTOCOL);

        if (socket_fd == -1) {
            cerr << "Failed to create socket. Error: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }

        if (DEBUG_MODE) {
            cout << "socket(" << ADDRESS_FAMILY << ", " << connection_type << ", " << PROTOCOL << ");" << endl;
        }
    } else {
        cerr << "NULL pointer: ADDRESS_FAMILY or connection_type or port is NULL" << endl;
    }
}

void Server::setSocketOptions() {
    const int trueFlag = 1;

    int status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int));

    if (status < 0) {
        cerr << "Setsockopt failed. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket() {
    socket_address.sin_family = ADDRESS_FAMILY;
    socket_address.sin_addr.s_addr = ALLOWED_IP;
    socket_address.sin_port = htons(port);

    int status = bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address));

    if (status < 0) {
        cerr << "Failed to bind to port '" << port << "'. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Server::startListening() {
    int status;
    status = listen(socket_fd, BACKLOG_QUEUE_SIZE);

    if (status < 0) {
        cerr << "Failed to listen on socket. Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    fcntl(socket_fd, F_SETFL, O_NONBLOCK);  // marking socket as nonblocking
}

void Server::handleClients() {
    grabConnection();
}

void Server::grabConnection() {
    size_t address_length = sizeof(socket_address);
    int client_socket_fd = accept(socket_fd, (struct sockaddr*)&socket_address, (socklen_t*)&address_length);

    if (client_socket_fd < 0) {
        // cerr << "Failed to accept connection. Error: " << strerror(errno) << endl;

    } else {
        char client_ip[IP_STRING_LENGTH];
        inet_ntop(ADDRESS_FAMILY, &(socket_address.sin_addr), client_ip, IP_STRING_LENGTH);

        if (DEBUG_MODE) {
            cout << "Server: Connection from: " << client_ip << endl;
        }

        struct pollfd poll_fd;
        poll_fd.fd = client_socket_fd;
        poll_fd.events = POLLIN;
        poll_fd.revents = 0;
    }
}

// getters

int Server::getIpStringLength(int version) {
    if (version == 4) {
        return INET_ADDRSTRLEN;

    } else if (version == 6) {
        return INET6_ADDRSTRLEN;

    } else {
        cerr << "Invalid ip version :" << version << endl;
        exit(EXIT_FAILURE);
    }
}

// setters

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
        this->connection_type = SOCK_STREAM;
        if (DEBUG_MODE) {
            cout << "TCP selected" << endl;
        }

    } else if (!strcmp(connectionType, "UDP")) {
        this->connection_type = SOCK_DGRAM;
        if (DEBUG_MODE) {
            cout << "UDP selected" << endl;
        }

    } else {
        cerr << "Invalid type for connection: " << connectionType << endl;
        exit(EXIT_FAILURE);
    }
}