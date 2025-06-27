#include <coroutine>
#include <iostream>

// Custom awaiter that always suspends
struct SimpleAwaiter {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "Suspended...\n";
    }
    void await_resume() const noexcept {
        std::cout << "Resumed.\n";
    }
};

// Coroutine return type
struct Task {
    struct promise_type {
        Task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// Coroutine function
Task my_coroutine() {
    std::cout << "Before co_await\n";
    co_await SimpleAwaiter{};
    std::cout << "After co_await\n";
}

int main() {
    my_coroutine();
    return 0;
}
