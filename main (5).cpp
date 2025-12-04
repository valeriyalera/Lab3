#include <iostream>
#include <vector>
#include <thread>
#include <latch> 
#include <syncstream> 
#include <chrono>

const int NT = 4;

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

void f(char name, int index) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::osyncstream(std::cout) << "From set " << name << " action was performed " << index << ".\n";
}

int main() {
    std::osyncstream(std::cout) << "Calculation started.\n";
    std::latch to_start_e{ 2 };
    std::latch to_start_fg{ 2 };
    std::latch to_start_h{ 2 };
    std::latch to_start_i{ 2 };
    std::latch to_start_j{ 2 };

    std::vector<std::jthread> Threads;
    Threads.reserve(NT);

    for (int i = 0; i < NT; i++) {

        Threads.emplace_back([i, &to_start_e, &to_start_fg, &to_start_h, &to_start_i, &to_start_j]() {

            if (i == 0) {
                for (int k = 1; k <= count_a; k++) f('a', k);
                to_start_e.count_down();
            }
            else if (i == 1) {
                for (int k = 1; k <= count_b; k++) f('b', k);
                to_start_e.count_down();
                to_start_fg.count_down();
            }
            else if (i == 2) {
                for (int k = 1; k <= count_c; k++) f('c', k);
                to_start_fg.count_down();
                to_start_h.count_down();
            }
            else if (i == 3) {
                for (int k = 1; k <= count_d; k++) f('d', k);
                to_start_h.count_down();
            }


            if (i == 0) {
                to_start_e.wait();
                for (int k = 1; k <= count_e; k++) f('e', k);
                to_start_i.count_down();
            }
            else if (i == 1) {
                to_start_fg.wait();
                for (int k = 1; k <= count_f; k++) f('f', k);
                to_start_i.count_down();
            }
            else if (i == 2) {
                to_start_fg.wait();
                for (int k = 1; k <= count_g; k++) f('g', k);
                to_start_j.count_down();
            }
            else if (i == 3) {
                to_start_h.wait();
                for (int k = 1; k <= count_h; k++) f('h', k);
                to_start_j.count_down();
            }


            if (i == 0) {
                to_start_i.wait();
                for (int k = 1; k <= count_i; k++) f('i', k);
            }
            else if (i == 3) {
                to_start_j.wait();
                for (int k = 1; k <= count_j; k++) f('j', k);
            }

            });
    }

    Threads.clear();

    std::osyncstream(std::cout) << "Calculation finished.\n";
    return 0;
}