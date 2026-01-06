#include "message.h"

Message::Message(int id, string text) : senderId(id), content(text){
    timestamp = time(nullptr);
}

int Message::getSenderId() const{
    return senderId; 
}
    
string Message::getContent() const{ 
    return content; 
}
    
string Message::getTimestampString() const{
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &timestamp);
    return string(buffer);
}