#include "storage.h"

void Storage::createUser(string name, int id){
    lock_guard<mutex> lock(storageMutex);
    users[id] = make_unique<User>(id, name);
}

void Storage::deleteUser(int userId){
    lock_guard<mutex> lock(storageMutex);
    if(users.erase(userId)){
        cout<<"User "<<userId<<" has been deleted"<<endl;
    }
    else{
        cout<<"User not found"<<endl;
    }
}

User* Storage::getUser(int id){
    lock_guard<mutex> lock(storageMutex);
    if(users.find(id) != users.end()) return users[id].get();
    else return nullptr;
}

string Storage::getUserName(int id){
    lock_guard<mutex> lock(storageMutex);
    if(users.find(id) != users.end()) return users[id]->getName();
    else return "User not found";
}

int Storage::createGroupChat(string groupName, const vector<int>& memberIds){
    lock_guard<mutex> lock(storageMutex);
    auto group = make_shared<GroupChat>(chatCounter, groupName);
    for(int id : memberIds) group->addMember(id);
    chats[chatCounter] = group;
    return chatCounter++;
}

shared_ptr<Chat> Storage::getOrCreateDirectChat(int u1, int u2){
    lock_guard<mutex> lock(storageMutex);
    for(auto const& [id, chat] : chats){
        shared_ptr<DirectChat> pChat = dynamic_pointer_cast<DirectChat>(chat);
        if(pChat && pChat->isBetween(u1, u2)){
            return chat;
        }
    }

    shared_ptr<DirectChat> newChat = make_shared<DirectChat>(chatCounter, u1, u2);
    chats[chatCounter] = newChat;
    chatCounter++;
    return newChat;
}

shared_ptr<Chat> Storage::getChat(int chatId){
    lock_guard<mutex> lock(storageMutex);
    if(chats.find(chatId) != chats.end()) return chats[chatId];
    return nullptr;
}

void Storage::showAllUsers(){
    lock_guard<mutex> lock(storageMutex);
    cout<<"\tUsers\n";
    for(const auto& pair : users){
        cout<<"ID: "<<pair.first<<"  Name: "<<pair.second->getName()<<endl;
    }
}