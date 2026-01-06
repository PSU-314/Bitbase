#include "user.h"

User::User(int userId, string userName) : id(userId), name(userName){}

int User::getId() const{
    return id; 
}
    
string User::getName() const{
    return name; 
}