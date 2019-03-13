## Key Facilities

`platinum`的网络层面参考`muduo`进行设计, 但是由于需求不同, 添加了自定义的设施.
并且对`muduo`原设计上进行了一定程度上的修改, 基本上已经与`muduo`有一定程度上的区别了
在抽象描述上仍遵循`muduo::EventLoop`, `muduo::Channel`, `muduo::Epoll`等对`Reactor`的抽象描述

下面是`platinum`中提供的一些关键的设施(Key Facility)

### platinum::Connection

Connection是`platinum`中最核心的类, 用来描述一条TCP Connection, 见`net/connection.h`

```cpp
#include <memory>
#include <functional>

#include "net/ip_address.h"
#include "protocol/parser.hpp"
#include "utility/noncopyable.h"
#include "utility/buffer/buffer.h"
#include "utility/buffer/writequeue.h"

namespace platinum {

class Socket;
class IPAddress;
class EventLoop;
class Channel;
class Connection : public Noncopyable {
 public:
 
  Connection(EventLoop *loop, int fd, ParserType type);
  ~Connection() override;

  ........

  void set_forward_fd(int fd);
  int forward_fd();
  int socket_fd();

 private:

  .....
  Buffer read_buffer_;
  WriteQueue write_queue_;
  int forward_fd_;
};

}

#endif //PLATINUM_NET_CONNECTION_H
```

#### ForwardFd

`platinum`作为WebServer. 同时还需要支持FCGI请求 (不限制)此时, WebServer, 作反向代理, 需要获取到进行转发的对应连接.

`platinum::TcpServer::connections_`以`fd_`作为`std::map`的键.可以通过静态方法`TcpServer::GetConnection(int forward_fd)` 获取到指定连接进行操作.

那么有其他办法吗? **有的**

对于FCGI, 我们在`FCGI_END_REQUEST`块中可以获取到`id`, 如果我们每次将对端连接作为`id`, 则可以在解析中获取到对端连接标识. 但是这种方法有一定的局限性: 特定于FCGI协议, 万一我们使用WSGI协议, 那就僵住了.
所以, 为了一定程度上的扩展性, 多维护一个fd标识也是可以接受的, 
在不过分修改内核参数的情况下,`int`类型(本机是`int32_t`)可以保存下`id`

### platinum::WriteQueue

WriteQueue是`platinum`在写操作方面于`muduo`的不同之处. 见`utility/buffer/write_queue.h`

需求分析: 
对于WebServer, 大多数时候还需要支持大文件的发送. 
静态请求可以直接将文件进行发送, 通过中介缓冲区进行`::write`转发, 会有一定程度上的效率损失, 

结论:
1. 对于短字符串我们可以直接`::write`, 
2. 大文件我们使用`::sendfile(64)`可以直接避过缓冲区.一定程度上还能降低内存占用

那么, 问题来了, 如果同时维护: `WriteBuffer`, `SendfileBuffer`. 我们使用了两个相同的设施维护写操作, 设计有问题. 同时这样设计最严重的问题是:**我们在同一条Connection上进行写操作, 使用两个结构进行维护, 会消息乱序**

最后的实现是:
 `platinum::Task`作为任务基类接口, 分别实现`platinum::WriteTask`, `platinum::SendTask`.
 使用`platinum::WriteQueue`统一维护任务, 保存写入状态. 使用智能指针保存当前无法完全写入的数据
写任务队列的核心实现是`bool platinum::TaskQueue::DoTask()`
```cpp
bool WriteQueue::DoTask()
{
  // if we complete all the task, we should get out.
  while (!queue_.empty()) {
    // complete the task, std::queue::front() => reference, 
    //this can change the state of the elements in the container.
    if (queue_.front()->operator()()) {
      queue_.pop_front();
    } else {
    // non-complete the task, e.g. EAGAIN / EWOULDBLOCK
      break;
    }
  }
  return queue_.empty();
}
```

其中`platinum::WritTask::operator()()`两者实现基本类似
```cpp
bool WriteTask::operator()()
{
  while (true) {
    auto var = ::write(fd_, data_.get() + completed_, remained_);
    if (var == remained_) {
      return true;
    } else if ( var >= 0 && var < remained_) {
      remained_ -= var;
      completed_ += var;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;
      } else {
        LOG(ERROR) << "WriteTask::operator() => Write Error";
        std::abort();
      }
    }
  }
}
```

可扩展性: 如果有新的写操作, 我们可以直接继承`platinum::Task`, 之后实现自身的调用运算符重载
最后实现`platinum::TaskQueue::TaskInQueue()`的重载形式即可

### Platinum::Parser

关于`platinum::Parser`就涉及到如何处理消息的设计了. `muduo`本身是只处理TCP字节流收发的
`platinum`却要完成两种协议的支持, 甚至有可能需要三种协议的实现, 那么, 就不能把消息处理写死在编码中. 最终选择的方案是, 给每个`Connection`内置一个`Parser`.

不同的协议支持不同的`Paser`.在连接建立之初就指定这条连接上的协议. 就可以轻松地解决需求了.

对于`Parser`, 我们设置为抽象基类, 用户自定义继承即可. 保证有`long Parser::Feed(...)`, `bool Parser::Complete()`等基本接口即可

*目前`platinum`的实现未完全修改为继承体系, 所以在使用的时候使用了`static_cast<>`, 这实属不安全的行为, 准确的应该是`dynamic_cast<>`, 不过使用虚函数机制, 可以完全规避这个问题*

在platinum中, 基本协议的支持是这样实现的 (因为只是服务端, 所以只是半协议实现)
见`protocol/http/request_parser.h`及`protocol/fastCGI/response_parser.h`

```cpp
// HTTP Protocol
namespace http {

class Request;
class RequestParser : public platinum::Parser {
 public:
  using const_iter = std::vector<HttpData>::const_iterator;
  ......
 private:
  .....
};

}

// FCGI Protocol
namespace fcgi {

enum State : int {
  COMPLETED,
  UNCOMPLETED,
  FAULT,
};

 class ResponseParser : public platinum::Parser {
 public:
  using const_iter = std::vector<FCGIData>::const_iterator;
  ......
};

}
```
在`platinum::Connection`中这样支持就可以: 见`net/connection.cc`

```cpp
Connection::Connection(EventLoop *loop, int fd, ParserType type)
    : loop_(loop),
      socket_(std::make_unique<Socket>(fd)),           // use move constructor
      channel_(std::make_unique<Channel>(loop, fd)),
      parser_(std::make_unique<http::RequestParser>()) {
  switch (type) {
    case ParserType::FCGI:
      parser_ = std::make_unique<fcgi::ResponseParser>(); break;
    case ParserType::HTTP:
      parser_ = std::make_unique<http::RequestParser>(); break;
    default:
      parser_ = std::make_unique<http::RequestParser>(); break;
  }
}
```
### platinum::Connector

`Connector`是用来进行反向代理的, 大体上和`Connection`类似
会将创建的`Connection`置于`TcpServer::connections_`中

详细见`net/connector.h`

```cpp
class Connector {
 public:
  using MessageCallback = std::function<long (Connection *, Buffer &, std::unique_ptr<platinum::Parser> &)>;
  using WriteableCallback = std::function<void()>;

  Connector(EventLoop *loop, const IPAddress &address, ParserType type);
  Connector(EventLoop *loop, const UnixAddress &address, ParserType type);
  ~Connector() = default;

  void StartNewConnection();
  
  int fd() const { return fd_; }
  int forward_fd() const { return forward_fd_; }
  void set_forward_fd(int fd) { forward_fd_ = fd; }
  auto connection_ptr() const -> std::shared_ptr<Connection>;
  .......
 private:
  .......
  int forward_fd_;
  std::shared_ptr<Connection> connection_ptr_;
  WriteableCallback write_callback_;
  MessageCallback message_callback_;
};
```

使用非阻塞的`::connect(2)`, 将连接是否成功的判断, 放在事件回调中处理.
```cpp
void Connector::StartNewConnection()
{
  ::connect(fd_, address_->sockaddr_ptr(), address_->sockaddr_len());
  platinum::TcpServer::NewConnection(this);
}

// 当非阻塞的Connector有事件之时, 进行判断
void Connector::HandleEvent()
{
  int error{};
  socklen_t len(sizeof(int));
  int ret = ::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &error, &len);
  if (ret < 0) {
    LOG(ERROR) << "Connector::HandleWrite() => ::getsockopt() Error";
    std::abort();
  }

  if (error) {
    switch (error) {
      case EAGAIN:
      case EINPROGRESS:
        break;
      default:
        LOG(ERROR) << "Connector::HandleWrite() => Connect Error";
    }
  }
  // TODO: Add selfconnect check out + reconnect mechanism
}
```

所有的读写等操作委托于`Connector::connection_ptr`

创建委托给TcpServer
```cpp
auto TcpServer::NewConnector(const IPAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return t_tcp_server->NewConnectorImpl(address, type);
}

auto TcpServer::NewConnectorImpl(const IPAddress &address, ParserType type) -> std::shared_ptr<Connector>
{
  return std::make_shared<Connector>(loop_, address, type);
}
```

整体上绕的有些复杂, **归根结底, 还是最初设计上的纰漏..**
