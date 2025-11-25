// var17, Potiekhina Valeriia K-27
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

class MyLatch {
    int count;
    std::mutex m;
    std::condition_variable cv;
public:
    explicit MyLatch(int n) : count(n) {}

    void count_down() {
        std::lock_guard<std::mutex> lock(m);
        if (count > 0) {
            --count;
            if (count == 0) cv.notify_all();
        }
    }

    void wait() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]{ return count == 0; });
    }
};

std::mutex print_mutex;

const int count_a = 4;
const int count_b = 4;
const int count_c = 4;
const int count_d = 4;
const int count_e = 4;
const int count_f = 5;
const int count_g = 4;
const int count_h = 8;
const int count_i = 5;
const int count_j = 8;

const int nt = 4;

MyLatch b_completed(1);

MyLatch c_completed(1);

MyLatch i_dependencies(3);

MyLatch j_dependencies(3);


void f(char name, int index) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "З набору " << name << " виконано дію " << index << ".\n";
}

void thread1_func() {
    for (int k = 1; k <= count_b; ++k) f('b', k);
    b_completed.count_down(); 

    for (int k = 1; k <= count_e; ++k) f('e', k);
    i_dependencies.count_down();
    
    i_dependencies.wait();
    for (int k = 1; k <= count_i; ++k) f('i', k);
}

void thread2_func() {
    for (int k = 1; k <= count_a; ++k) f('a', k);
    i_dependencies.count_down();

    for (int k = 1; k <= count_c; ++k) f('c', k);
    c_completed.count_down();

    for (int k = 1; k <= count_g; ++k) f('g', k);
    i_dependencies.count_down();
}


void thread3_func() {
    for (int k = 1; k <= count_d; ++k) f('d', k);
    j_dependencies.count_down();

    c_completed.wait();
    for (int k = 1; k <= count_h; ++k) f('h', k);
    j_dependencies.count_down();
}

void thread4_func() {
    b_completed.wait();
    for (int k = 1; k <= count_f; ++k) f('f', k);
    j_dependencies.count_down();

    j_dependencies.wait();
    for (int k = 1; k <= count_j; ++k) f('j', k);
}

int main() {
    std::cout << "Обчислення розпочато.\n";

    std::thread t1(thread1_func);
    std::thread t2(thread2_func);
    std::thread t3(thread3_func);
    std::thread t4(thread4_func);

    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
    if (t3.joinable()) t3.join();
    if (t4.joinable()) t4.join();

    std::cout << "Обчислення завершено.\n";
    return 0;
}