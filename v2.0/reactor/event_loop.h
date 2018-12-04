//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_EVENTLOOP_H
#define PLATINUM_EVENTLOOP_H

#include <atomic>
#include <thread>
#include <functional>
#include <vector>
#include <mutex>

namespace platinum {

class Channel;
class EPoller;
class EventLoop {
 public:
  // All Functor or Callback, use this std::function format.
  // You can use std::bind, lambda expression ( I suggest you use lambda expression !)
  using Functor = std::function<void ()>;

  EventLoop();
  ~EventLoop();

  void Loop();                                 // The most important interface of EventLoop, to loop !
  void RunInLoop(Functor cb);                  // One of the other important interface, allow other thread move their task into this IO thread

  void Quit();                                 // if want to quit loop, use this interface(you may not need this indeed

  bool AssertInLoopThread();                   // judge if the components in the same thread of evnetloop.

 private:

  // to wake up EventLoop when it block on EventLoop::loop()
  // specifically, use eventfd(2) trigger readable event to wake up
  void WakeUp();
  void HandleWakeUp();

  void DoPendingFunctors();                    // execute other thread task in IO thread, in EventLoop::loop()

  void AbortInLoopThread();                    // Log ERROR and abort programm.
  bool IsInLoopThread();

  int wakeup_fd_;                              // fd:        wakeup_fd_ , to wakeup IO thread, when block on EventLoop::loop()->poll;
  std::unique_ptr<Channel> wakeup_channel_;    // channel:   use this to register into epoller and handle read event.
  std::unique_ptr<EPoller> poller_;            // poller:    IO Mulplexing
  std::vector<Functor> pending_functors_;      // vector:    store the functors, which other threads want to excute in IO thread
  std::vector<Channel *> active_channels_;     // channel:   IO Mulplexing return the active channels;
  std::atomic<bool> looping_;                  // bool:      to record the looping state, for ~EventLoop()
  std::atomic<bool> quit_;                     // bool:      to record loop() state, use this to judge if go to next loop;
  std::mutex mutex_;                           // mutex:     for pending_functors_ , this may cause race-condition.
  std::thread::id thread_id_;                  // thread_id: record the EventLoop thread, which make it one loop per thread
};

}

#endif //PLATINUM_EVENTLOOP_H
