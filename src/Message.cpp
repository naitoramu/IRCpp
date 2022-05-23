#include "Message.hpp"
#include "Server.hpp"

#include <sstream>
#include <iostream>
#include <utility>

Message::Message(string message) {
    this->message = std::move(message);

    processMessage();
}

void Message::processMessage() {
    splitMessageByLines();
    parseMessage();
}

void Message::splitMessageByLines() {
    stringstream ss(message);
    string line;

    while(getline(ss, line, '\n')) {
        splitted_message.push_back(line);
    }

}

void Message::parseMessage() {
    parsedLine parsed_line;
    for (const string& line : splitted_message) {
        parsed_line.command.clear();
        parsed_line.text.clear();
        if (!line.empty()) {

            string::size_type pos = line.find(' ');

            if (pos != string::npos) {
                string first_word = line.substr(0, pos);
                string rest = line.substr(pos + 1);

                if (Server::isValidCommand(first_word)) {
                    parsed_line.command = first_word;
                    parsed_line.text = rest;

                } else {
                    parsed_line.text = line;

                }
            } else if (Server::isValidCommand(line)) {
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