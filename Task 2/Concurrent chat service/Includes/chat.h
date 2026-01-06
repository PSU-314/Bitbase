#ifndef CHAT_H
#define CHAT_H

#include <vector>
#include <string>
#include <algorithm>
#include "message.h"
#include <mutex>

using namespace std;

class Chat{
protected:
    int chatId;
    vector<Message> history;
    mutable mutex chatMutex;

public:
    Chat(int id);
    virtual ~Chat(){}

    int getId() const;
    
    void addMessage(Message msg);

    const vector<Message> getHistory() const;

    virtual bool hasUser(int userId) = 0;
    virtual string getChatType() = 0;
};

class DirectChat : public Chat{
    int user1Id;
    int user2Id;

public:
    DirectChat(int id, int u1, int u2);

    bool hasUser(int userId) override;

    bool isBetween(int u1, int u2);

    string getChatType() override;
};


class GroupChat : public Chat{
    string groupName;
    vector<int> memberIds;
    mutable mutex memberMutex;

public:
    GroupChat(int id, string name);

    void addMember(int userId);

    bool hasUser(int userId) override;

    string getChatType() override;
};

#endif