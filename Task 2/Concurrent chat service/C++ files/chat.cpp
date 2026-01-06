#include "chat.h"

Chat::Chat(int id) : chatId(id){}

int Chat::getId() const{ 
    return chatId; 
}

void Chat::addMessage(Message msg){
    lock_guard<mutex> lock(chatMutex);
    history.push_back(msg);
}

const vector<Message> Chat::getHistory() const{
    lock_guard<mutex> lock(chatMutex);
    return history;
}

DirectChat::DirectChat(int id, int u1, int u2) : Chat(id), user1Id(u1), user2Id(u2){}

bool DirectChat::hasUser(int userId){
    return userId == user1Id || userId == user2Id;
}

bool DirectChat::isBetween(int u1, int u2){
    return (user1Id == u1 && user2Id == u2) || (user1Id == u2 && user2Id == u1);
}

string DirectChat::getChatType(){ 
    return "Direct Chat"; 
}

GroupChat::GroupChat(int id, string name) : Chat(id), groupName(name){}

void GroupChat::addMember(int userId){
    lock_guard<mutex> lock(memberMutex);
    memberIds.push_back(userId);
}

bool GroupChat::hasUser(int userId){
    lock_guard<mutex> lock(memberMutex);
    for(int id : memberIds){
        if(id == userId) return true;
    }
    return false;
}

string GroupChat::getChatType(){ 
    return "Group chat"; 
}