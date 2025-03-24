#include <iostream>
#include <thread>
#include <mutex>

class Counter {
    int counter = 0;
    std::mutex mutex;

public:
    void inc() {
        // step 1:
        // mutex.lock();
        // counter++;
        // mutex.unlock();

        std::scoped_lock lock (mutex);

        // throw "imag_issue";

        // issue => std::scoped_lock (mutex);
        // Foo f(1);
        // Foo(1);

        counter++;
    }

    int get() const {
        return counter;
    }
};

int main() {
    Counter c;
    const int interation_value = 10000;

    auto work = [&c]() {
        for (int i = 0; i < interation_value; i++) {
            c.inc();
        }
    };

    std::thread t( work);
    std::thread t1( work);
    t1.join();
    t.join();

    std::cout << "ctr: " << c.get() << std::endl;

    return 0;
}