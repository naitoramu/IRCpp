#pragma once

#include <string>
#include <vector>

#include "CommandHandler.hpp"

using namespace std;

class CommandHandler;

struct parsedLine {
    string command;
    string text;
};

class Message {

private:

    CommandHandler *command_handler;
    string message;
    vector<string> splitted_message;
    vector<parsedLine> parsed_message;

public:

    Message(CommandHandler *);

    ~Message();

    void parseMessage(string);

    vector<parsedLine> getParsedMessage();

private:

    void processMessage();

    void splitMessageByLines();

    void parse();

};