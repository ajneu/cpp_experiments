/*
Only difference to main.cpp is: we pack the thread and condition_variable into a struct together and use only one vector
(here we also don't call send)
 */

#include <iostream>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <chrono>
#include <atomic>

unsigned n;
std::mutex mut;
std::atomic<bool> done{false};

void func(std::condition_variable &cv)
{
    while (true) {
        std::unique_lock<std::mutex> lk(mut);
        cv.wait(lk);

        if (done)
            break;
        
        std::cout << n++ << std::endl;
    }     
}

struct ThCv{
    std::thread th;
    std::condition_variable cv;
};

int main()
{
    constexpr unsigned num_threads = 10;
    constexpr unsigned high_cnt = 30;

    std::vector<ThCv> vec(num_threads); // http://stackoverflow.com/a/28305867

    for (unsigned i = 0; i < num_threads; ++i) {
        vec[i].th = std::thread(std::bind(func, std::ref(vec[i].cv)));
    }

    for (unsigned i = 0; i < high_cnt; ++i) {
        vec[i%num_threads].cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }

    done = true;
    for (auto &v : vec) {
        v.cv.notify_one();
    }
    
    for (auto &v : vec) {
        v.th.join();
    }
    return 0;
}
