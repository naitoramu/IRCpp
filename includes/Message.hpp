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
public:
    Message(CommandHandler *);
    ~Message();

    void parseMessage(string);
    void displayParsedMessage();
    void handleMessage();

    vector<parsedLine> getParsedMessage();

private:
    CommandHandler *command_handler;
    string message;
    vector<string> splitted_message;
    vector<parsedLine> parsed_message;

    void processMessage();
    void splitMessageByLines();
    void parse();
};