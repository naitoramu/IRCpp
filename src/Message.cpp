#include "Message.hpp"

#include <sstream>
#include <iostream>

Message::Message(CommandHandler *command_handler) {
    this->command_handler = command_handler;
}

void Message::parseMessage(string message) {
    this->message = message;

    processMessage();
}

void Message::processMessage() {
    splitMessageByLines();
    parse();
}

void Message::splitMessageByLines() {

    splitted_message.clear();
    stringstream ss(message);
    string line;

    while (getline(ss, line, '\n')) {
        splitted_message.push_back(line);
    }

}

void Message::parse() {
    parsed_message.clear();
    parsedLine parsed_line;
    for (string &line: splitted_message) {

        parsed_line.command.clear();
        parsed_line.text.clear();

        if (!line.empty()) {

            string::size_type pos = line.find(' ');

            if (pos != string::npos) {
                string first_word = line.substr(0, pos);
                string rest = line.substr(pos + 1);

                if (command_handler->isValidCommand(first_word)) {
                    parsed_line.command = first_word;
                    parsed_line.text = rest;

                } else {
                    parsed_line.text = line;

                }
            } else if (command_handler->isValidCommand(line)) {
                parsed_line.command = line;

            } else {
                parsed_line.text = line;

            }
            parsed_message.push_back(parsed_line);

        }
    }
}

// getters
vector<parsedLine> Message::getParsedMessage() {
    return parsed_message;
}

// destructor
Message::~Message() = default;