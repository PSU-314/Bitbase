#ifndef STORAGE_H
#define STORAGE_H

#include <map>
#include <memory>
#include <iostream>
#include "user.h"
#include "chat.h"

using namespace std;

class Storage{
    map<int, unique_ptr<User>> users;
    map<int, shared_ptr<Chat>> chats;
    
    int chatCounter = 1;
    mutable mutex storageMutex;

public:
    void createUser(string name, int id);

    void deleteUser(int userId);

    User* getUser(int id);

    string getUserName(int id);

    int createGroupChat(string groupName, const vector<int>& memberIds);

    shared_ptr<Chat> getOrCreateDirectChat(int u1, int u2);

    shared_ptr<Chat> getChat(int chatId);

    void showAllUsers();
};

#endif