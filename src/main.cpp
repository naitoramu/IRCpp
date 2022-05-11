#include <csignal>
#include <iostream>

#include "Config.cpp"
#include "Server.h"
#include "User.h"

bool working = true;

void sigHandler(int signum) {
    (void)signum;
    working = false;
}

int main(int argc, char *argv[]) {
    vector<Channel> channels;
    channels.push_back(Channel(1));
    channels.push_back(Channel(2));

    vector<Channel> channels2;
    channels2.push_back(Channel(1));
    channels2.push_back(Channel(3));
    channels2.push_back(Channel(4));

    vector<User> users;
    users.push_back(User("maruś", "haslo123", &channels));
    users.push_back(User("czaruś", "silnehaslo", &channels2));

    Config<User> config("config/users.csv");
    config.write(&users);
    // Server server = Server(4, 2137);

    // signal(SIGINT, sigHandler);

    // while(working) {
    // 	server.handleClients();
    // }

    std::cout << "Done" << std::endl;
}