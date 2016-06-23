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


void send(unsigned n)
{
    static std::mutex send_mut;
    std::lock_guard<std::mutex> lk{send_mut};

    std::this_thread::sleep_for(std::chrono::milliseconds{100});
    std::cout << n++ << std::endl;
}

void func(std::condition_variable &cv)
{
    while (true) {
        {
            std::unique_lock<std::mutex> lk(mut);
            cv.wait(lk);
            
            if (done)
                break;
        }

        send(n++); //blocking call (outside of lock lk, so that multiple threads can run into send)
    }     
}

int main()
{
    constexpr unsigned num_threads = 10;
    constexpr unsigned high_cnt = 30;

    std::vector<std::thread> vec_th;
    std::vector<std::condition_variable> vec_cv(num_threads); // http://stackoverflow.com/a/28305867


    for (unsigned i = 0; i < num_threads; ++i) {
        vec_th.emplace_back(std::thread(std::bind(func, std::ref(vec_cv[i]))));
    }

    for (unsigned i = 0; i < high_cnt; ++i) {
        vec_cv[i%num_threads].notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds{100}); /* Sleep to ensure no thread is stuck in mutex send_mut; when we send notify_one below */

    done = true;
    for (auto &v : vec_cv) {
        v.notify_one();
    }

    unsigned i = 0;
    for (auto &v : vec_th) {
        v.join();
    }
    return 0;
}
