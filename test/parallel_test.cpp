#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "parallel/Worker.hpp"

TEST(ParallelTests, test1) {
  WorkerPool wpool(3);

  int task_count = 100;
  std::mutex m;
  int tasks_done = 0;
  for (int i = 0; i < task_count; i++) {
    wpool.add_task([&tasks_done, &task_count, &wpool, &m, i]() {
      std::cout << "Task " << i << std::endl;
      unsigned long r = 0;
      for (unsigned long j = 0; j < (1UL << 12UL); j++) {
        r += (r + j * i + j + i) ^ r;
      }
      std::cout << "r is " << r << std::endl;
      std::lock_guard lg(m);
      tasks_done++;
      if (tasks_done == task_count) {
        wpool.stop_all_workers();
      }
    });
  }

  wpool.wait_workers();
}