/*
 * Created by Crow on 8/21/18.
 * Description: The class of thread_pool
 * API:
 *          (non)  thread_pool(int num);           // Constructor (thread num)
 *          (non)  ~thread_pool();                 // Destructor
 *          bool   submit_task();                  // Submit task to the pool
 *          void   release();                      // Discarded, move to thread_pool::~thread_pool();
 */


#ifndef PLATINUMSERVER_THREAD_POOL_H
#define PLATINUMSERVER_THREAD_POOL_H

#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include <atomic>
#include "blocked_queue.hpp"

namespace PlatinumServer {
  class thread_pool {
  public:
      using task = std::function<void()>;

      explicit thread_pool(int num = std::thread::hardware_concurrency());
      thread_pool(const thread_pool &) = delete;
      thread_pool &operator=(const thread_pool &) = delete;
      ~thread_pool();
      bool submit_task(task &&);
  private:
      int thread_num;
      std::vector<std::thread> thread_vector;
      std::atomic<bool> flag;
      PlatinumServer::blocked_queue<task> task_queue;

      static int thread_max_num() { return 1024; }
      void execute();
      void release();
  };
}

#endif //PLATINUMSERVER_THREAD_POOL_H
