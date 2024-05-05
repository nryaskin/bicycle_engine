#include <iostream>
#include <coroutine>
#include <vector>

struct promise {
    std::vector<int> vec;
    std::vector<int> get_return_object() {
        std::cout << "get_return_object" << std::endl;
        return vec;
    }
    std::suspend_never initial_suspend() noexcept {
        std::cout << "initial_suspend" << std::endl;
        return {};
    }
    std::suspend_never final_suspend() noexcept {
        std::cout << "final_suspend" << std::endl;
        return {};
    }
    void return_value(std::vector<int>&& value) {
        std::cout << "return_value" << std::endl;
        for (auto& v: value) {
            std::cout << v << " \n";
        }
        vec = std::move(value);
    }
    void unhandled_exception() {}
};

struct coroutine : std::coroutine_handle<::promise> {
    using promise_type = ::promise;
};

std::vector<int> get_vector() {
    return { 1, 2, 3, 4};
}

int main() {
    auto vec = get_vector();
    std::cout << "size: " << vec.size() << std::endl;
    for (auto &v: vec) {
        std::cout << v << "\n";
    }
    return 0;
}
