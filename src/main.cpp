#include <csignal>
#include <iostream>

#include "Server.hpp"

bool working = true;
unsigned int User::id_counter = 1;

void sigHandler(int signum) {
    (void) signum;
    working = false;
}

int main(int argc, char *argv[]) {

    Server server(2137);

    signal(SIGINT, sigHandler);

    while (working) {

        server.grabConnection();
        server.handleClients();

    }

    std::cout << "Server turned off" << std::endl;
}
