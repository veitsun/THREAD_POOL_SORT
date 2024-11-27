#pragma once
#include "SafeQueue.h"
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class ThreadPool {
private:
  class ThreadWorker {
  private:
    ThreadPool *m_pool;
    int m_id;

  public:
    ThreadWorker(ThreadPool *pool, const int id);
    void operator()();
  };

  std::vector<std::thread> m_threads;
  bool m_shutdown;
  SafeQueue<std::function<void()>> m_queue;
  std::mutex m_conditional_mutex;
  std::condition_variable m_conditional_lock;

public:
  ThreadPool(const int n_threads);
  ~ThreadPool() { shutdown(); }

  // Prevent copying and moving
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;

  void init();
  void shutdown();

  // Fully implemented template method IN THE HEADER
  template <typename F, typename... Args>
  auto submit(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    // Create a function with bounded parameters ready to execute
    std::function<decltype(f(args...))()> func =
        std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    // Encapsulate it into a shared ptr in order to be able to copy construct /
    // assign
    auto task_ptr =
        std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
    // Wrap packaged task into void function
    std::function<void()> wrapper_func = [task_ptr]() { (*task_ptr)(); };
    // Enqueue generic wrapper function
    m_queue.enqueue(wrapper_func);
    // Wake up one thread if its waiting
    m_conditional_lock.notify_one();
    // Return future from promise
    return task_ptr->get_future();
  }
};