#include <thread>
#include <future>
#include <iostream>
#include <chrono>

int add(int a, int b) {
    using namespace std::chrono_literals;
    std::cout << std::this_thread::get_id() << ": add start" << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // std::this_thread::sleep_for(std::chrono::duration<double, std::chrono::seconds>(1.5));
    // std::this_thread::sleep_for(1h);
    // std::this_thread::sleep_for(30min);
    std::this_thread::sleep_for(300ms);
    std::cout << std::this_thread::get_id() << ": add end" << std::endl;
    return a + b;
};

int main() {
    std::cout << std::this_thread::get_id() << ": main start" << std::endl;
    // auto result = std::async(add, 1, 3);
    // auto result = std::async(std::launch::async, add, 1, 3);
    auto result = std::async(std::launch::deferred, add, 1, 3);
    std::cout << std::this_thread::get_id() << ": main after sync" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << std::this_thread::get_id() << ": main after sleep" << std::endl;
    std::cout << "ctr: " << result.get() << std::endl;
    return 0;
};
