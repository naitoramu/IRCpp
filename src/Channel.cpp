#include "Channel.h"

Channel::Channel() : ID(id_counter++) {
}

Channel::Channel(unsigned int id) : ID(id) {
}

Channel::~Channel() {
}

unsigned int Channel::getID() {
    return ID;
}

// unsigned int Channel::id_counter = 0;