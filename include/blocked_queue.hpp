/*
 * Created by Crow on 8/21/18.
 * Description: 阻塞队列实现
 * API:
 *      (non)   blocked_queue<T>();      // Constructor
 *      (non)   ~blocked_queue();        // Destructor
 *      bool    push(T &&data);          // push data to the queue
 *      bool    pop(T &val);             // pop and get the data (Because of the wait_for() and notify(), transmit the l-val
 */

#include <iostream>
#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace PlatinumServer {
  template <typename T>
  class blocked_queue {
  public:
      blocked_queue() = default;
      bool push(T &&data);
      bool pop(T &val);
  private:
      std::mutex mtx;                        // single-lock to block between push() & pop()
      std::condition_variable cv;
      std::queue<T> queue;

      bool empty();
  };

  template <typename T>
  bool blocked_queue<T>::push(T &&data)
  {
      std::lock_guard<std::mutex> lock(mtx);                      // blocked the pop();
      queue.emplace(std::move(data));
      cv.notify_one();

      return true;
  }

  template <typename T>
  bool blocked_queue<T>::pop(T &val)
  {
      std::unique_lock<std::mutex> lock(mtx);                     // lock_guard isn't suitable for wait();

      /*
       * the same as:
       * while (empty()) {
       *      cv.wait(lock);
       * }
      */
      cv.wait_for(lock, std::chrono::microseconds(100), [&]() { return !empty(); });
      if (empty()) {
          return false;
      }
      else {
          val = queue.front();
          queue.pop();
          lock.unlock();
          return true;
      }
  }

  template <typename T>
  bool blocked_queue<T>::empty()
  {
      return queue.empty();                       // only use empty() in pop(), have gotten the mutex;
  }
}
