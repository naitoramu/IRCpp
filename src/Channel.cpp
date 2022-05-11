#include "Channel.h"

Channel::Channel(unsigned int id) : ID(id) {
}

Channel::~Channel() {
}

unsigned int Channel::getID() {
    return ID;
}