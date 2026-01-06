#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User{
    int id;
    string name;

public:
    User(int userId, string userName);

    int getId() const;
    
    string getName() const;
};

#endif