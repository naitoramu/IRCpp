#include <csignal>
#include <iostream>

#include "Config.cpp"
#include "Server.h"
#include "User.h"

bool working = true;
unsigned int Channel::id_counter = 5;

void sigHandler(int signum) {
    (void)signum;
    working = false;
}

int main(int argc, char *argv[]) {

    // vector<unsigned int> channels;
    // channels.push_back(1);
    // channels.push_back(2);

    // vector<unsigned int> channels2;
    // channels2.push_back(3);
    // channels2.push_back(4);
    // channels2.push_back(5);

    // vector<User> users;
    // users.push_back(User("maruś", "haslo123", &channels));
    // users.push_back(User("czaruś", "silnehaslo", &channels2));

    // vector<User> * users;
    // Config<User> config("users.csv");
    // config.write(&users);
    // users = config.read();

    // for (User user : *users) {
    //     cout << user.getUsername() << ',' << user.getPassword() << endl;
    //     // cout << user.getChannels() << endl;
    //     for (unsigned int channel_id : *(user.getChannels())) {
    //         cout << channel_id << ',';
    //     }

    //     cout << endl;
    // }
    Server server = Server(4, 2137);

    signal(SIGINT, sigHandler);

    while(working) {
    	server.handleClients();
    }

    std::cout << "Done" << std::endl;
}