#include "../include/ThreadPool.h"

// ThreadWorker constructor implementation
ThreadPool::ThreadWorker::ThreadWorker(ThreadPool *pool, const int id)
    : m_pool(pool), m_id(id) {}

// ThreadWorker operator() implementation
void ThreadPool::ThreadWorker::operator()() {
  std::function<void()> func;
  bool dequeued;
  while (!m_pool->m_shutdown) {
    {
      std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);
      if (m_pool->m_queue.empty()) {
        m_pool->m_conditional_lock.wait(lock);
      }
      dequeued = m_pool->m_queue.dequeue(func);
    }
    if (dequeued) {
      func();
    }
  }
}

// ThreadPool constructor implementation
ThreadPool::ThreadPool(const int n_threads)
    : m_threads(std::vector<std::thread>(n_threads)), m_shutdown(false) {}

// Initialize thread pool
void ThreadPool::init() {
  for (int i = 0; i < m_threads.size(); ++i) {
    m_threads[i] = std::thread(ThreadWorker(this, i));
  }
}

// Shutdown thread pool
void ThreadPool::shutdown() {
  m_shutdown = true;
  m_conditional_lock.notify_all();

  for (int i = 0; i < m_threads.size(); ++i) {
    if (m_threads[i].joinable()) {
      m_threads[i].join();
    }
  }
}

// Note: The submit method is a template and must remain in the header file