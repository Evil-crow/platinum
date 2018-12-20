//
// Created by Crow on 12/11/18.
//

#ifndef PLATINUM_TCP_CONNECTION_H
#define PLATINUM_TCP_CONNECTION_H

#include <memory>
#include <functional>

#include "net/ip_address.h"
#include "utility/noncopyable.h"
#include "utility/buffer/buffer.h"
#include "utility/buffer/writequeue.h"

namespace platinum {

class Socket;
class IPAddress;
class EventLoop;
class Channel;
class TCPConnection : public Noncopyable {
 public:
  using EventCallback = std::function<void ()>;
  using MessageCallback = std::function<bool (TCPConnection *, Buffer &)>;
  using CloseCallback = std::function<void (int)>;

  TCPConnection(EventLoop *loop,
                int fd,
                const IPAddress &local_address,
                const IPAddress &peer_address);
  ~TCPConnection() override;

  // move TCPConnection callback to Channel
  void ConnectionEstablished();
  void SendData(const void *data, size_t length);

  // copy TCPServer callback -> TCPConnection, can't move, we'll create much TCPConnection by TCPServer::*callback
  void SetConnectionCallback(const EventCallback &callback);
  void SetMessageCallback(const MessageCallback &callback);
  void SetCloseCallback(const CloseCallback &callback);

 private:
  void HandleRead();                                   // deal with readable event, doing by Channel::read_callback_
  void HandleWrite();                                  // deal with writeable event, doing by Channel::write_callback_
  void HandleError();                                  // deal with error, ...
  void HandleClose();                                  // deal with hangup event, doing by Channel::close_callback_

  void ErrorCallback();                                // private function to record the error situation

  EventLoop *loop_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  EventCallback connection_callback_;                  // register by user provide function -> OnConnection fucntion
  MessageCallback message_callback_;                   // register by user provide function -> OnMessage Function
  CloseCallback close_callback_;                       // register as TCPServer::RemoveConnection
  Buffer read_buffer_;
  WriteQueue write_queue_;
  IPAddress local_addrss_;
  IPAddress peer_address_;
};

}

#endif //PLATINUM_TCP_CONNECTION_H
