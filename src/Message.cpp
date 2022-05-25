#include "Message.hpp"
#include "Server.hpp"

#include <sstream>
#include <iostream>
#include <utility>

Message::Message(CommandHandler *command_handler) {
    this->command_handler = command_handler;
}

void Message::parseMessage(string message) {
    this->message = message;
//    cout << "Message: " << message << endl;

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

    while(getline(ss, line, '\n')) {
        splitted_message.push_back(line);
    }

}

void Message::parse() {
    parsed_message.clear();
    parsedLine parsed_line;
    for (string& line : splitted_message) {
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

void Message::displayParsedMessage() {
    int line_counter = 0;
    for (const parsedLine& parsed_line : parsed_message) {
        cout << line_counter << ". " << parsed_line.command << " <" << parsed_line.text << ">" << endl;
        line_counter++;
    }
}

// getters
vector<parsedLine> Message::getParsedMessage() {
    return parsed_message;
}

Message::~Message() = default;