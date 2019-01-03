//
// Created by Crow on 11/25/18.
//

#ifndef PLATINUM_EVENTLOOP_H
#define PLATINUM_EVENTLOOP_H

#include <mutex>
#include <atomic>
#include <thread>
#include <vector>
#include <functional>
#include <net/connection.h>

namespace platinum {

class Channel;
class EPoller;
class EventLoop {
 public:
  // All Functor or Callback, use this std::function<void ()> format.
  // You can use std::bind, lambda expression ( I suggest you use lambda expression !)
  using Functor = std::function<void ()>;

  EventLoop();
  ~EventLoop();

  void Loop();                                         // The most important interface of EventLoop, to loop !
  void Quit();                                         // if want to quit loop, use this interface(you may not need this indeed
  void AddChannel(Channel *channel);                   // Actually use EPoller::AddChannel()
  void UpdateChannel(Channel *channel);                // Actually use EPoller::UpdateChannel()
  void RemoveChannel(int fd);                          // Actually use EPoller::RemoveChannel()

  bool AssertInLoopThread();                           // judge if the components in the same thread of evnetloop.
  void RunInLoop(const Functor &cb);                         // One of the other important interface, allow other thread move their task into this IO thread

 private:
  // to wake up EventLoop when it block on EventLoop::loop()
  // specifically, use eventfd(2) trigger readable event to wake up
  void WakeUp();
  void HandleWakeUp();
  void QueueInLoop(const Functor &cb);
  void DoPendingFunctors();                                    // execute other thread task in IO thread, in EventLoop::loop()

  void AbortInLoopThread();                                    // Log ERROR and abort programm.
  bool IsInLoopThread();

  std::atomic<bool> looping_;                                  // bool:      to record the looping state, for ~EventLoop()
  std::atomic<bool> quit_;                                     // bool:      to record loop() state, use this to judge if go to next loop;
  std::atomic<bool> eventing_handling_;                        // bool:      to record event handling state
  int wakeup_fd_;                                              // fd:        wakeup_fd_ , to wakeup IO thread, when block on EventLoop::loop()->poll;
  std::unique_ptr<EPoller> epoller_;                           // epoller:    IO Mulplexing
  std::shared_ptr<Channel> wakeup_channel_;                    // channel:   use this to register into epoller and handle read event.
  std::vector<Channel *> active_channels_;
  std::mutex mutex_;                                           // mutex:     for pending_functors_ , this may cause race-condition.
  std::thread::id thread_id_;                                  // thread_id: record the EventLoop thread, which make it one loop per thread

  std::atomic<bool> calling_pending_functor_;                  // bool:      to record the state
  std::vector<Functor> pending_functors_;                      // vector:    store the functors, which other threads want to excute in IO thread
};



}

#endif //PLATINUM_EVENTLOOP_H
