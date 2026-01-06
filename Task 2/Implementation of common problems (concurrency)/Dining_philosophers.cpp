// Solution using resource hierarchy

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

using namespace std;

mutex forks[5];
mutex outputMutex;

void print(const string& output){
    lock_guard<mutex> lock(outputMutex);
    cout<<output<<endl;
}

void philosopher(int id){
    int left = id;
    int right = (id+1)%5;

    // Philosopher with the highest ID picks up the forks in reverse order(right then left).
    if(id==4){
        swap(left, right);
    }

    print("Philosopher " + to_string(id) + " is thinking");
        
    // Pick up forks
    lock_guard<mutex> lock1(forks[left]);
    lock_guard<mutex> lock2(forks[right]);

    print("Philosopher " + to_string(id) + " is eating");
    this_thread::sleep_for(chrono::milliseconds(500)); 

    print("Philosopher " + to_string(id) + " finished eating");
}

int main(){
    vector<thread> philosophers;

    // Creating 5 philosopher threads
    for(int i=0; i<5; i++){
        philosophers.push_back(thread(philosopher, i));
    }

    // Wait for all threads to finish
    for(auto& t : philosophers){
        t.join();
    }

    return 0;
}