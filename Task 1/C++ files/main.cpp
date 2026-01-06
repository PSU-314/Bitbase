#include <iostream>
#include <vector>
#include "main.h"
#include "storage.h"

Storage db;

void createUser(){
    string name;
    int id;
    cout<<"Enter Name: "; 
    cin>>name;
    cout<<"Enter the ID: ";
    cin>>id;

    if(db.getUser(id) == nullptr){
        db.createUser(name, id);
        cout<<"User created"<<endl;
    }
    else cout<<"User ID not unique"<<endl;
}

void deleteUser(){
    int uid;
    cout<<"Enter User ID to delete: ";
    cin>>uid;
    db.deleteUser(uid);
}

void handleDirectChat(){
    int senderId, receiverId;
    cout<<"Enter the User ID: "; 
    cin>>senderId;
    cout<<"Enter receiver User ID: "; 
    cin>>receiverId;

    if(!db.getUser(senderId) || !db.getUser(receiverId)){
        cout<<"Invalid User IDs."<<endl;
        return;
    }

    auto chat = db.getOrCreateDirectChat(senderId, receiverId);
    
    string msgContent;
    cout<<"Enter message: ";
    cin.ignore(); 
    getline(cin, msgContent);

    chat->addMessage(Message(senderId, msgContent));
    cout<<"Message sent in Chat ID "<<chat->getId()<<endl;
}

void handleGroupChat(){
    int senderId, chatId;
    cout<<"Enter your User ID: "; 
    cin>>senderId;

    if(db.getUser(senderId) == nullptr){
        cout<<"Invalid User ID"<<endl;
        return;
    }
    
    cout<<"Enter Group Chat ID: "; 
    cin>>chatId;

    auto chat = db.getChat(chatId);

    if(!chat){
        cout<<"Chat not found"<<endl;
        return;
    }

    if(!(chat->hasUser(senderId))){
        cout<<"You are not a member"<<endl;
        return;
    }

    string msgContent;
    cout<<"Enter message: ";
    cin.ignore();
    getline(cin, msgContent);
    chat->addMessage(Message(senderId, msgContent));
    cout<<"Message sent to group.\n";
}

void createGroup(){
    string name;
    cout<<"Enter Group Name: ";
    cin.ignore();
    getline(cin, name);

    int count;
    cout<<"Enter the number of members: "; 
    cin>>count;
    vector<int> members;
    for(int i=0; i<count; i++){
        int uid;
        cout<<"Enter Member "<<i+1<<" User ID: "; 
        cin>>uid;
        if(db.getUser(uid) != nullptr) members.push_back(uid);
        else{
            cout<<"Invalid User ID \nEnter again:"<<endl;
            i--;
        }
    }
    int gid = db.createGroupChat(name, members);
    cout<<"Group '"<<name<<"' created with ID: "<<gid<<endl;
}

void showHistory(){
    int chatId;
    cout<<"Enter Chat ID to view history: "; 
    cin>>chatId;
    auto chat = db.getChat(chatId);
    
    if(!chat){
        cout<<"Chat not found."<<endl;
        return;
    }

    cout<<"\nHistory for "<<chat->getChatType()<<"\n";
    for(const auto& msg : chat->getHistory()){
        string senderName = db.getUserName(msg.getSenderId());
        cout<<"["<<msg.getTimestampString()<<"] "<<senderName<<": "<<msg.getContent()<<endl;
    }
    cout<<endl;
}

int main(){
    int choice;
    while(true){
        cout<<"\n\tMEMORY CHAT SERVICE\n";
        cout<<"1. Create User\n";
        cout<<"2. Create Group\n";
        cout<<"3. Direct Chat(Send Msg)\n";
        cout<<"4. Group Chat(Send Msg)\n";
        cout<<"5. Delete User\n";
        cout<<"6. View Message Log\n";
        cout<<"7. List All Users\n";
        cout<<"0. Exit\n";
        cout<<"Select: ";
        cin>>choice;

        switch(choice){
            case 1: createUser(); 
                    break;
            case 2: createGroup(); 
                    break;
            case 3: handleDirectChat(); 
                    break;
            case 4: handleGroupChat(); 
                    break;
            case 5: deleteUser(); 
                    break;
            case 6: showHistory(); 
                    break;
            case 7: db.showAllUsers(); 
                    break;
            case 0: return 0;
            default: cout<<"Invalid choice"<<endl;
        }
    }
}