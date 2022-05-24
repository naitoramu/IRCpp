#include "Server.hpp"
#include "Message.hpp"

#include <arpa/inet.h>
#include <cstring>

#include <iostream>

using namespace std;

Server::Server(int port) {

    socket = new Socket(port);
    socket->startListening();

    command_handler = new CommandHandler(this);
}

void Server::grabConnection() {
    size_t address_length = sizeof(socket->getSockAddrIn());
    int client_socket_fd = accept(socket->getFileDescriptor(), (struct sockaddr*)socket->getSockAddrIn(), (socklen_t*)&address_length);

    if (client_socket_fd >= 0) {
        char client_ip[IP_STRING_LENGTH];

        inet_ntop(socket->getAddressFamily(), socket->getInAddr(), client_ip, IP_STRING_LENGTH);

        cout << "Server: Connection from: " << client_ip << endl;

        pollfd poll_fd{};
        poll_fd.fd = client_socket_fd;
        poll_fd.events = POLLIN;
        poll_fd.revents = 0;

//        cout << "Connected fd: " << poll_fd.fd << endl;
        unique_ptr<User> new_user = make_unique<User>(poll_fd);
        connected_users.push_back(move(new_user));
        connected_fds.push_back(poll_fd);
//        cout << "FD:" << connected_users.at(0)->poll_fd.fd << endl;
    }
//    for (auto & connected_user : connected_users) {
//    }

    // cout << "Connected users count: " << connected_fds.size() << endl;
}

void Server::handleClients() {

    int readable_descriptors = poll(connected_fds.data(), connected_fds.size(), INACTIVITY_TIMEOUT);

    char buffer[1024];
    const int trueFlag = 0;

    if (readable_descriptors > 0) {
        for (pollfd poll_fd : connected_fds) {
            if (poll_fd.revents & POLLIN) {
                int received_value = recv(poll_fd.fd,
                                          buffer,
                                          sizeof(buffer),
                                          trueFlag);

                if (received_value < 0) {
                    if (errno != EWOULDBLOCK) {
                        cerr << "Error while reading. Error:" << strerror(errno) << endl;
                    }

                } else if (received_value == 0) {
                    cerr << "Conection closed by client" << endl;

                } else {

                    cout << "Received " << received_value << " bytes from fd: " << poll_fd.fd << endl;

                    auto *message = new  Message(buffer);
                    User *user = getUserByFD(poll_fd.fd);
                    message->displayParsedMessage();
//                    user->handleMessage(message);
                    command_handler->handleMessage(message, user);

                    delete message;
                }

                poll_fd.revents = 0;
                memset(buffer, 0, sizeof(buffer));
            }
        }
    }
}

bool Server::isValidCommand(string command) {

    if (command.at(0) == '/') {
        command = command.substr(1);
    }

    for (const string& available_command : available_commands) {
        if (command == available_command) {
            return true;
        }
    }

    return false;
}

// getters

string Server::getConnectedToServerUserNicknames() {
    string nicknames = "";

    if(connected_users.size() == 1) {
        return "Just you";
    }

    for (auto & connected_user : connected_users) {
        nicknames += (connected_user->getNickname() + '\n');
    }

    return nicknames;
}

User * Server::getUserByFD(int file_descriptor) {
    cout << "Connected users: " << connected_users.size() << endl;

    for (auto & connected_user : connected_users) {
        cout << "user.poll_fd.fd = " << connected_user->poll_fd.fd << endl;
        if (connected_user->poll_fd.fd == file_descriptor) { return connected_user.get(); }
    }

    cerr << "Cannot find user with fd == " << file_descriptor << endl;
    exit(EXIT_FAILURE);
}

Server::~Server() {
    delete socket;
}