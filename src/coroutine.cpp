#include <chrono>
#include <coroutine>
#include <functional>
#include <iostream>
#include <queue>
#include <thread>

void enqueue_ready(std::function<void()> fn);

struct task {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;
  handle_type coro;

  task(handle_type h) : coro(h) {}
  ~task() {
    if (coro)
      coro.destroy();
  }

  void start() { coro.resume(); }

  struct promise_type {
    auto get_return_object() { return task{handle_type::from_promise(*this)}; }
    std::suspend_always initial_suspend() { return {}; }
    std::coroutine_handle<> continuation;
    auto final_suspend() noexcept {
      struct final_awaiter {
        bool await_ready() noexcept { return false; }
        void await_suspend(std::coroutine_handle<promise_type> h) noexcept {
          if (h.promise().continuation)
            h.promise().continuation.resume();
        }
        void await_resume() noexcept {}
      };
      return final_awaiter{};
    }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    // Store the continuation and start the inner task
    coro.promise().continuation = h;
    coro.resume();
  }

  void await_resume() const noexcept {}
};

struct timer_awaitable {
  int delay_ms;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::coroutine_handle<> h) {
    int delay_ms = this->delay_ms;
    std::thread([delay_ms, h]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
      enqueue_ready([h]() { h.resume(); });
    }).detach();
  }

  void await_resume() const noexcept {
    std::cout << "Timer awaitable resumed" << std::endl;
  }
};

std::queue<std::function<void()>> ready_queue;

void enqueue_ready(std::function<void()> fn) {
  ready_queue.push(std::move(fn));
}

void run_event_loop() {
  while (!ready_queue.empty()) {
    auto fn = std::move(ready_queue.front());
    ready_queue.pop();
    fn();
  }
}

task async_main() {
  // Write main logic here
  std::cout << "Start async task\n";
  co_await timer_awaitable{1000};
  std::cout << "Resumed after 1 second" << std::endl;
}

task async_main_wrapper() {
  co_await async_main();
  std::exit(0);
}

int main() {
  auto t = async_main_wrapper();
  t.start();

  // Fake event loop: wait for detached threads to enqueue resumption
  while (true) {
    run_event_loop();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
