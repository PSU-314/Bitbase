#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <ctime>
#include <iostream>

using namespace std;

class Message{
    int senderId;
    string content;
    time_t timestamp;

public:
    Message(int id, string text);

    int getSenderId() const;
    
    string getContent() const;
    
    string getTimestampString() const;
};

#endif