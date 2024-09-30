#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Global variables for synchronization
std::mutex mtx;
std::condition_variable cv;
bool firstThreadDone = false;  // Condition flag

// Function for the first thread to count up from 0 to 20
void countUp() {
    std::lock_guard<std::mutex> lock(mtx); // Lock the mutex during critical section
    for (int i = 0; i <= 20; ++i) {
        std::cout << "Counting up: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work with a small delay
    }
    firstThreadDone = true;  // Signal that the first thread is done
    cv.notify_one();         // Notify the second thread to start counting down
}

// Function for the second thread to count down from 20 to 0
void countDown() {
    std::unique_lock<std::mutex> lock(mtx); // Lock the mutex during critical section
    cv.wait(lock, []{ return firstThreadDone; }); // Wait for the first thread to finish

    for (int i = 20; i >= 0; --i) {
        std::cout << "Counting down: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Simulate work with a small delay
    }
}

int main() {
    // Create two threads
    std::thread t1(countUp);  // Thread 1 counts up
    std::thread t2(countDown); // Thread 2 counts down

    // Join the threads with the main thread to ensure they complete
    t1.join();
    t2.join();

    return 0;
}
