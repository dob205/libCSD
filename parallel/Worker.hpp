#ifndef WORKER_HPP
#define WORKER_HPP

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class WorkerQueue {
  std::mutex mutex;
  std::deque<std::function<void()>> q;

public:
  void add_task(std::function<void()> &fun) {
    std::lock_guard lg(mutex);
    q.push_back(fun);
  }

  bool empty() {
    std::lock_guard lg(mutex);
    return q.empty();
  }

  std::function<void()> pop() {
    std::lock_guard lg(mutex);
    auto fun = q.front();
    q.pop_front();
    return fun;
  }
};

class Worker {
  WorkerQueue &queue;
  std::mutex &shared_mutex;
  std::condition_variable &queue_cv;

  std::mutex mutex_stop;

  std::unique_ptr<std::thread> th;
  bool _stopped;

  int worker_id;

public:
  Worker(WorkerQueue &queue, std::mutex &shared_mutex,
         std::condition_variable &queue_cv)
      : queue(queue), shared_mutex(shared_mutex), queue_cv(queue_cv),
        _stopped(false) {
    static int workers_count = 0;
    worker_id = workers_count++;
    start();
  }

  void join() { th->join(); }

  void stop() { set_stopped(true); }

private:
  void start() { th = std::make_unique<std::thread>(&Worker::run, this); }

  bool stopped() {
    std::lock_guard lg(mutex_stop);
    return _stopped;
  }

  void set_stopped(bool next_value) {
    std::lock_guard lg(mutex_stop);
    _stopped = next_value;
  }

  void run() {
    while (!stopped()) {
      std::unique_lock<std::mutex> ul(shared_mutex);
      queue_cv.wait(ul, [this]() { return stopped() || !queue.empty(); });
      if (stopped())
        return;
      if (queue.empty())
        continue;
      auto task = queue.pop();
      ul.unlock();
      queue_cv.notify_all();
      task();
    }
  }
};

class WorkerPool {
  WorkerQueue queue;
  std::vector<std::unique_ptr<Worker>> workers;

  std::mutex shared_mutex;
  std::condition_variable queue_cv;

public:
  WorkerPool(int workers_amount) {
    for (int i = 0; i < workers_amount; i++) {
      workers.push_back(
          std::make_unique<Worker>(queue, shared_mutex, queue_cv));
    }
  }

  void add_task(std::function<void()> &&task) {
    queue.add_task(task);
    queue_cv.notify_all();
  }

  void wait_workers() {
    for (auto &w : workers)
      w->join();
  }

  void stop_all_workers() {
    for (auto &w : workers)
      w->stop();
    queue_cv.notify_all();
  }
};

#endif /* WORKER_HPP */
