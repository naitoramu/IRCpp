#include <csignal>
#include <iostream>

#include "Server.hpp"

bool working = true;

void sigHandler(int signum) {
    (void) signum;
    working = false;
}

unsigned int User::id_counter = 1;

int
main(int argc, char *argv[]) {
    Server server(2137);

    signal(SIGINT, sigHandler);
    while (working) {

        server.grabConnection();
        server.handleClients();
    }

    std::cout << "Done" << std::endl;
}
