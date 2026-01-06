#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>

using namespace std;

int sharedData = 0;
int readCount = 0;

mutex resourceMutex;
mutex countMutex;
mutex outputMutex;

void print(const string& output){
    lock_guard<mutex> lock(outputMutex);
    cout<<output<<endl;
}

void reader(int id){
    for(int i=0; i<3; i++){
        // Entry Section
        {
            lock_guard<mutex> lock(countMutex);
            readCount++;
            if(readCount == 1){
                resourceMutex.lock(); // First reader locks the data from writers
            }
        }

        // Reading Section
        print("Reader " + to_string(id) + " is reading: " + to_string(sharedData));
        this_thread::sleep_for(chrono::milliseconds(500));

        // Exit Section
        {
            lock_guard<mutex> lock(countMutex);
            readCount--;
            if(readCount == 0){
                resourceMutex.unlock(); // Last reader unlocks the data for writers
            }
        }
        
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void writer(int id){
    for(int i=0; i<2; i++){
        // Entry Section
        resourceMutex.lock(); // Writer needs exclusive access

        // Writing Section
        sharedData += 7;
        print("Writer " + to_string(id) + " updated data to " + to_string(sharedData));
        this_thread::sleep_for(chrono::milliseconds(1000));

        // Exit Section
        resourceMutex.unlock();

        this_thread::sleep_for(chrono::milliseconds(1500));
    }
}

int main(){
    vector<thread> readers;
    vector<thread> writers;

    // 3 Readers and 2 Writers threads
    for(int i=1; i<=3; i++) readers.push_back(thread(reader, i));
    for(int i=1; i<=2; i++) writers.push_back(thread(writer, i));

    for(auto& t : readers) t.join();
    for(auto& t : writers) t.join();

    return 0;
}