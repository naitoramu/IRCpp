#pragma once

#include <string>
#include <vector>

using namespace std;

struct parsedLine {
    string command;
    string text;
};

class Message {
public:
    Message(string message);
    ~Message();

    void displayParsedMessage();
    void handleMessage();

    vector<parsedLine> getParsedMessage();

private:
    string message;
    vector<string> splitted_message;
    vector<parsedLine> parsed_message;

    void processMessage();
    void splitMessageByLines();
    void parseMessage();
};