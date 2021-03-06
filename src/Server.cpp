#include "Server.hpp"
#include "Message.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <algorithm>

#include <iostream>

using namespace std;

Server::Server(int port) {

    socket = new Socket(port);
    command_handler = new CommandHandler(this);
    message = new Message(command_handler);

    socket->startListening();
    defineChannels();
}

void Server::defineChannels() {
    channels.emplace_back("#channel1");
    channels.emplace_back("#channel2");
    channels.emplace_back("#channel3");
    channels.emplace_back("#channel4");
    channels.emplace_back("#channel5");
}

void Server::grabConnection() {

    size_t address_length = sizeof(socket->getSockAddrIn());
    int client_socket_fd = accept(socket->getFileDescriptor(),
                                  (struct sockaddr *) socket->getSockAddrIn(),
                                  (socklen_t *) &address_length);

    if (client_socket_fd >= 0) {
        char client_ip[IP_STRING_LENGTH];

        inet_ntop(socket->getAddressFamily(), socket->getInAddr(), client_ip, IP_STRING_LENGTH);

        cout << "Server: Connection from: " << client_ip << endl;

        pollfd poll_fd{};
        poll_fd.fd = client_socket_fd;
        poll_fd.events = POLLIN;
        poll_fd.revents = 0;

        unique_ptr<User> new_user = make_unique<User>(poll_fd);
        connected_users.push_back(move(new_user));
        connected_fds.push_back(poll_fd);

    }
}

void Server::handleClients() {

    int readable_descriptors = poll(connected_fds.data(),
                                    connected_fds.size(),
                                    INACTIVITY_TIMEOUT);

    if (readable_descriptors > 0) {
        handleWaitingClients();

    }
}

void Server::handleWaitingClients() {
    for (pollfd poll_fd: connected_fds) {

        if (poll_fd.revents & POLLIN) {
            int received_value = recv(poll_fd.fd,
                                      messageBuffer,
                                      sizeof(messageBuffer),
                                      0);

            if (received_value < 0) {
                cerr << "Error while reading. Error:" << strerror(errno) << endl;

            } else if (received_value == 0) {
                cerr << "Conection closed by client" << endl;
                removeDissconnectedClient(poll_fd.fd);

            } else {

                cout << "Received " << received_value << " bytes from fd: " << poll_fd.fd << endl;

                User *user = getUserByFileDescriptor(poll_fd.fd);

                message->parseMessage(messageBuffer);
                command_handler->handleMessage(message, user);
            }

            poll_fd.revents = 0;
            memset(messageBuffer, 0, sizeof(messageBuffer));
        }
    }
}

void Server::removeDissconnectedClient(int &fd) {
    removeDisconnectedFileDescriptor(fd);
    removeDisconnectedUser(fd);
}

void Server::removeDisconnectedFileDescriptor(int &fd) {
    for (int i = 0; i < connected_fds.size(); i++) {
        if (connected_fds[i].fd == fd) {
            connected_fds.erase(next(connected_fds.begin(), i));
            cout << "Disconnected file descriptor removed" << endl;
            break;
        }
    }
    cerr << "Specified file descriptor '" << fd << "' not found" << endl;
}

void Server::removeDisconnectedUser(int &fd) {
    for (int i = 0; i < connected_users.size(); i++) {
        if (connected_users[i]->poll_fd.fd == fd) {
            connected_users.erase(next(connected_users.begin(), i));
            cout << "Disconnected file descriptor removed" << endl;
            break;
        }
    }
    cerr << "User with file descriptor = '" << fd << "' not found" << endl;
}

bool Server::channelExists(const string &channel_name) {

    if (count(channels.begin(), channels.end(), channel_name)) {
        return true;

    } else {
        return false;

    }
}

string Server::getConnectedToServerUserNicknames() {
    string nicknames = "Online on the server:\n\n";

    if (connected_users.size() == 1) {
        return nicknames + "Just you\n";
    }

    for (auto &connected_user: connected_users) {
        nicknames += (connected_user->getNickname() + '\n');
    }

    return nicknames;
}

string Server::getConnectedToChannelUserNickames(const string &channel_name) {
    string nicknames = "Online on " + channel_name + ":\n\n";
    int user_counter = 0;

    for (auto &connected_user: connected_users) {
        if (connected_user->getCurrentChannelName() == channel_name) {
            nicknames += (connected_user->getNickname() + '\n');
            user_counter++;
        }
    }

    if (!user_counter) {
        nicknames = "Nobody online on " + channel_name + "\n";
    }

    return nicknames;
}

string Server::getChannelNames() {
    string channel_names = "Available channels:\n\n";
    for (string &channel_name: channels) {
        channel_names += channel_name + "\n";
    }
    return channel_names;
}

vector<int> *Server::getUserOnChannelFileDescriptors(const string &channel_name) {
    vector<int> *file_descriptors = new vector<int>;

    for (unique_ptr<User> &user: connected_users) {
        if (user->getCurrentChannelName() == channel_name) {
            file_descriptors->push_back(user->poll_fd.fd);

        }
    }

    return file_descriptors;
}

User *Server::getUserByFileDescriptor(int file_descriptor) {

    for (auto &connected_user: connected_users) {
        if (connected_user->poll_fd.fd == file_descriptor) { return connected_user.get(); }
    }

    cerr << "Cannot find user with fd == " << file_descriptor << endl;
    exit(EXIT_FAILURE);
}

Server::~Server() {
    delete socket;
    delete command_handler;
    delete message;
}