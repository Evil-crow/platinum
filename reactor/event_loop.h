//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_EVENTLOOP_H
#define PLATINUM_EVENTLOOP_H

#include <thread>

namespace platinum {

class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  void loop();

  bool AssertInLoopThread();
  bool IsInLoopThread();

  private:
  void AbortInLoopThread();

  bool looping_;
  std::thread::id thread_id_;
};

}

#endif //PLATINUM_EVENTLOOP_H
