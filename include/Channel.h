#ifndef CHANNEL_H
#define CHANNEL_H

#pragma once

class Channel {
   public:
    Channel();
    Channel(unsigned int);
    ~Channel();

    // getters
    unsigned int getID();

   private:
    const unsigned int ID;
    static unsigned int id_counter;
};

#endif