#include <csignal>
#include <iostream>

#include "Server.hpp"

bool working = true;

void sigHandler(int signum) {
    (void) signum;
    working = false;
}

vector<string> defineAvailableCommands() {
    vector<string> available_commands;

    available_commands.emplace_back("HELLO");
    available_commands.emplace_back("JOIN");
    available_commands.emplace_back("LEAVE");
    available_commands.emplace_back("QUIT");
    available_commands.emplace_back("HELP");
    available_commands.emplace_back("WHO");
    available_commands.emplace_back("LIST");
    available_commands.emplace_back("NICK");

    return available_commands;
}

vector<string> Server::available_commands = defineAvailableCommands();
unsigned int User::id_counter = 1;

int
main(int argc, char *argv[]) {
    Server server(2137);

    signal(SIGINT, sigHandler);
    while (working) {
        // system("clear");

        server.grabConnection();
        server.handleClients();
    }

    std::cout << "Done" << std::endl;
}
