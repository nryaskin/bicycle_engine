#include <coroutine>
#include <iostream>
#include <cinttypes>

// https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html
struct ReturnObject {
    struct promise_type {
        ReturnObject get_return_object() { 
            std::cout << "ReturnObject::get_return_object\n";
            return {};
        }
        std::suspend_never initial_suspend() {
            std::cout << "ReturnObject::initial_suspend\n";
            return {};
        }
        std::suspend_never final_suspend() noexcept {
            std::cout << "ReturnObject::final_suspend\n";
            return {};
        }
        void unhandled_exception() {
            std::cout << "ReturnObject::unhandled_exception\n";
        }
    };
};

struct Awaiter {
    std::coroutine_handle<> *hp_;
    bool await_ready() const noexcept {
        std::cout << "Awaiter::await_ready\n";
        return false;
    }
    void await_suspend(std::coroutine_handle<> h) {
        std::cout << "Awaiter::await_suspend\n";
        *hp_ = h;
    }
    void await_resume() const noexcept {
        std::cout << "Awaiter::await_resume\n";
    }
};

ReturnObject counter(std::coroutine_handle<> *continuation_out) {
    std::cout << "counter>\n";
    Awaiter a { continuation_out };
    for (uint32_t i = 0;; ++i) {
        std::cout << "counter before co_await\n";
        co_await a;
        std::cout << "counter after co_await\n";
        std::cout << "counter: " << i << std::endl;
    }
    std::cout << "<counter\n";
}

int main() {
    std::coroutine_handle<> h;
    counter(&h);
    for (int i = 0; i < 2; ++i) {
        std::cout << "In main function\n";
        h();
    }
    h.destroy();
    return 0;
}
