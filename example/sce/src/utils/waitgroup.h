/**
 * \file
 * \brief Contains waitgroup sync primitive.
*/

#pragma once

// ============================================================================

#include <condition_variable>  // std::conditional_variable
#include <mutex>               // std::mutex

// ============================================================================

class Waitgroup {
 public:
  void Add(size_t work) {
    std::unique_lock lock(mutex_);
    work_ += work;
  }

  void Done() {
    std::unique_lock lock(mutex_);

    work_--;

    if (work_ == 0) {
      on_work_done_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock lock(mutex_);

    while (work_ > 0) {
      on_work_done_.wait(lock);
    }
  }

 private:
  size_t work_ = 0;
  std::mutex mutex_;
  std::condition_variable on_work_done_;
};
 