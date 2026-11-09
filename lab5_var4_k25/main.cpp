#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <syncstream>
#include <thread>

void compute(const std::string& name, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::osyncstream(std::cout) << name << '\n';
}
void slow(const std::string& name) { compute(name, 7); }
void quick(const std::string& name) { compute(name, 1); }

void work() {
    auto start = std::chrono::steady_clock::now();
    auto chainB = std::async(std::launch::async, [] {
        quick("B1");
        slow("B2");
        quick("B3");
        });
    quick("A1");
    slow("A2");
    quick("A3");
    chainB.get();
    quick("C");

    auto end = std::chrono::steady_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();
    std::osyncstream(std::cout) << "Time: " << sec << " s\n"
        << "Work is done!\n";
}

int main() {
    work();
}