// Solution using condition variables

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <queue>
#include <condition_variable>

using namespace std;

queue<int> buffer;
const int BUFFER_SIZE = 3;

mutex bufferMutex;
mutex outputMutex;
condition_variable cv;

void print(const string& output){
    lock_guard<mutex> lock(outputMutex);
    cout<<output<<endl;
}

void producer(int id){
    for(int i = 1;i<=5; i++){
        unique_lock<mutex> lock(bufferMutex);

        // If buffer is full, wait
        while(buffer.size() == BUFFER_SIZE){
            print("Buffer is full. Producer " + to_string(id) + " is waiting");
            cv.wait(lock);
        }

        // Add item
        buffer.push(i);
        print("Producer " + to_string(id) + " added item " + to_string(i));

        // Tell the Consumer to consume
        cv.notify_all();
        
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void consumer(int id){
    for(int i=1; i<=5; i++){
        unique_lock<mutex> lock(bufferMutex);

        // If buffer is empty, wait
        while(buffer.empty()){
            print("Buffer is empty. Consumer " + to_string(id) + " is waiting");
            cv.wait(lock);
        }

        // Remove item
        int val = buffer.front();
        buffer.pop();
        print("Consumer " + to_string(id) + " took item " + to_string(val));

        // Tell the Producer to produce"
        cv.notify_all();

        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

int main(){

    // 1 producer and 1 consumer
    thread p1(producer, 1);
    thread c1(consumer, 1);

    p1.join();
    c1.join();

    return 0;
}