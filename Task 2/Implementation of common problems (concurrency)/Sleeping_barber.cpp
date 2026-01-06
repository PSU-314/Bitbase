#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <condition_variable>

using namespace std;

const int CHAIRS = 3;
int waitingCustomers = 0;

mutex shopMutex;
mutex outputMutex;
condition_variable cv_barber;
condition_variable cv_customer;

void print(const string& output){
    lock_guard<mutex> lock(outputMutex);
    cout<<output<<endl;
}

void barber(){
    while(true){
        unique_lock<mutex> lock(shopMutex);

        // If no customers, the barber sleeps
        while(waitingCustomers == 0){
            print("Shop is empty");
            cv_barber.wait(lock);
        }

        // Customer present, move him from waiting chair to barber chair
        waitingCustomers--;
        print("Barber is cutting hair. (Waiting room: " + to_string(waitingCustomers) + " occupied)");
        
        // Signal the next customer that the barber is ready
        cv_customer.notify_one();
        
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(1000));
        print("Barber finished the haircut");
    }
}

void customer(int id){
    unique_lock<mutex> lock(shopMutex);

    if(waitingCustomers < CHAIRS){
        // Sit in a waiting chair
        waitingCustomers++;
        print("Customer " + to_string(id) + " sat down. (Waiting room: " + to_string(waitingCustomers) + " occupied)");

        // Wake up the barber if he is sleeping
        cv_barber.notify_one();

        // Wait for the barber to call customer
        cv_customer.wait(lock);
        print("Customer " + to_string(id) + " is getting their hair cut");
    } 
    
    else{
        // Shop is full, customer leaves
        print("Waiting room full. Customer " + to_string(id) + " left the shop");
    }
}

int main(){
    // Start the barber thread
    thread barberThread(barber);
    barberThread.detach(); // Running in the background

    vector<thread> customers;

    // Creating 6 customers
    for(int i=1; i<=6; i++){
        customers.push_back(thread(customer, i));
        this_thread::sleep_for(chrono::milliseconds(400));
    }

    for(auto& t : customers) t.join();
    
    return 0;
}