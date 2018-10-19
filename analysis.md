## PlatinumServer分析

环境: Fedora 27 + x86_64 + C++11
分析: ~~应XXX的响应, 进行代码分析,~~, 不过事实上来讲是真的有用 ( ՞ټ՞)

目录架构
```bash
PlatinumServer/
├── 404.html                           // 处理一般的无资源Response 404
├── CMakeLists.txt                     // 为了跨平台,使用cmake组织 (其实是Makefile不过关)
├── include
│   ├── blocked_queue.hpp              // 模板类实现阻塞队列
│   ├── epoll.h                        // 封装系统调用epoll
│   ├── http_affair.h                  // 处理HTTP事务, 事务逻辑
│   ├── http_response.h                // 继承httpxx, 构建适合自己的Response
│   ├── server.h                       // Server流程, global function
│   ├── socket.h                       // 封装Socket底层接口
│   └── thread_pool.h                  // 线程池的对应接口定义
├── LICENSE                            // MIT License
├── README.md
└── src                                // headers的对应实现
    ├── epoll.cc
    ├── http_affair.cc
    ├── http_response.cc
    ├── main.cc
    ├── server.cc
    ├── socket.cc
    └── thread_pool.cc
```

本篇分析并非按文件处理, 会从点(point)上来进行分析

## namespace

**目前使用: ** Platinumserver::

代码中使用了这样三个命名空间

```cpp
std::             (C++ Standard Library)
PlatinumServer::  (Project)
::                (global)
```
std是为了C++标准库设施, PlatinumServer是项目自身命名空间, ::全局命名空间是为了Syscall等

**优: **使用namespace可以最大可能避免名字冲突(~~说了句废话~~), 同时划分清楚组件范围
**劣: **过于冗长的名字, 则会有: 编码复杂, 无用代码过多, 而且经常是声明中使用, auto也用不了

**建议: **
1. 使用简洁, 短小的名字 e.g. std, tenserflow虽然也全称, 但还不够长
2. 如果非要使用全称,比如:PlatinumServer, 就可以使用using导入名字
`using namespace PlatinumServer`, 注意在`.cc`文件中, headers中导入名字, 是作死
3. 对于全局名字, 比如`::read`, `::sendfile64`, `::accept`, 
 建议: 如果没有明显冲突, 就可以不使用全局scope qualifier (反而, 难看, 多余)

## 命名规范

**目前使用: **下划线命名法 + 小写为主类名

Emmmmmm, 其实没有要求规范.选合适的就好

在C中, 下划线命名还是主流. ~~C++有的越看越不舒服...~~

**建议:**
1. 变量命名使用哪种形式呢, 合适就好.不过现在偏向驼峰命名法了(可能是Java写多了)
...

好了, 上面的东西可以暂时搁置一旁. **Google C++风格指南**大法好. 相当优秀
这个目前挖个坑, 希望下一版的设计能够用上 (无论是风格还是设计上), 以后的多人项目势必要统一风格

## 构造函数

在C++11之前, 某些手法真的是奇技淫巧, 现在有了C++11标准, 可以在下面这些方面上优化:
1. 尽可能使用`explicit`, 以此来避免隐式类型转换
2. `noexcept`可以作出无抛出的承诺, 在代码的生成中, 减少为可能throw execption而产出的代码
3. 使用意义明确的标识, 告诉编译器想法`=default`, `=delete`

## OOP程度不够 -> 设计风格不统一

虽然在EffectiveC++ Item 01中提到: 视C++为一个语言联邦
但是四(五)种编程范式: 面向对象 > 基于对象 > 面向过程 > 泛型编程

**所以, 我们使用面型对象程度高的对象化设计, 是有必要的**

比如:
```cpp
// platinumServer/src/main.cc
...
for (int i = 0; i < std::thread::hardware_concurrency() - 1; i++) {
	threads.emplace_back([] {
	  epoll_server(8090);
	});
}

epoll_server(8090);
...
```

上面的设计就不太好. `epoll_server()`完全就是一个全局函数, 一下破坏了OOP的风格设计

我们这样做更为恰当
```cpp
class Server {
public:
    Server (in_port_t port) : port(port) { ; }
    void run() {
        this->epoll_server();
	}
private:
    void epoll_server();
    in_port_t port;
};

void Server::epoll_server()
{
    ...
}

// User Code
...
Server server(port);
for (int i = 0; i < std::thread::hardware_concurrency() - 1; i++) {
	threads.emplace_back([] {
	  server.run();
	});
}

server.run();
```

使用粗糙的全局函数`void epoll_server()` 并没有 `server.run()` 来的更加合适, 对吧.

**对数据类型 in_port_t的封装也是同理**

## 关于Const

**传递Reference, 一定使用const, 如果没有修改的必要**

例如:
```cpp
// PlatinumServer/include/http_affair.h
bool is_static(http::Request &request);

const std::string get_dir(std::string &url);

const std::string get_filename(std::string &url);

bool get_request(http::Request &request, PlatinumServer::socket &conn_sock);

long int find_file(const std::string &request_url);

bool is_alive(http::Request &request);

bool build_response(PlatinumServer::http_response &response, http::Request &request);
```

**其中并非所有的引用都是需要修改的, 那么我们就有义务为他们都加上const限定符**
**同理, 成员函数尽可能的都使用const, 为了封装及保护成员**

## 关于R-Reference

右值引用是C++11新引入的概念, 我们需要注意的是: **传参R-V, 函数参数也得是R-V**

比如:
```cpp
// PlatinumServer/include/http_response.h
namespace PlatinumServer {
class http_response : public http::ResponseBuilder {
public:
    ...
    bool set_header(std::string &&key, std::string &&val);
    ...
};
}
```
我们强调的是: 调用时 `set_header(std::move(key), std::move(val))`
那么, 函数实现也一定得是使用右值引用, 否则, 右值过去转回左值, 代价反而飙升, 开销激增
`response.set_header("Content-Length", std::to_string(file_size));`
这是在使用时的, 一处调用. 已经违反了我们的建议: 字符串字面量不说了, 但是后面那个`string`对象
`response.set_header("Content-Length", std::move(std::to_string(file_size)));`

## 辅助函数设计

辅助函数,指的是实现功能的子函数.
EffC++在讨论, 避免模板参数造成代码膨胀时, 明确指出这个问题 (当然, 有点扯远了, 很多地方都讨论过)
**我们通过分析共性, 将公共实现部分抽离出来, 一方面保持优秀的设计结构, 一方面较少重复目标码生成**

但是, 要分清场合, ~~**过分设计就是傻逼**~~

```cpp
namespace PlatinumServer {
class socket {
public:
...
    void connect() {     // 大致实现
        bind();
        listen();
	}
...
private:
...
    bool bind();
    bool listen();
...
};
}
```

本来,在EffC++中的non-member设计时, 甚至还洋洋自满, 
因为`connect()`,访问私有实现, 某层意义上来说 -> **破坏了class的封装性**
要将`connect()`,设计为non-member function.

**没有注意到一个关键问题: **`bind()`**与**`listen()`**是否是真正意义上的辅助函数**

**事实上, 这两个只是被**`connect()`**使用, 没有被至少两个函数使用, 那么,直接实现在**`connect()`内部即可

也就没有之后的各种问题了, 不是吗 ?

## 关于err_handle

```cpp
inline void err_handle(const std::string &msg)
{
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}
```
短小精悍, 符合`inline`的需求. 同时又完美的使用`std::string::c_str()`转换?

**嗯 ? 转换 ? ? ?**. 这里实际上这样来写更合适

```cpp
inline void err_handle(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
```
因为我们的接口是, `void perror(const char *msg)`, 同时我们一般使用字面量

那么, 使用原来的写法就会造成: 字面量 -> `string` -> `char *str`
而后来的写法: `char *`可以直接指涉字面量, 避免中间过程的转换

**简单地说, 不要为了写C++而C++, C++成功的的一部分原因也是因为包容了C的大部分内容**

*其实这段话是强行洗地的, 如果C++完全自生, 类型统一, 也没有这回事*\

***

另外一点就是: 继承用得并不够, 当然也不能为了继承而继承

`<memory> std::shared_ptr, std::unique_ptr`,并没有太使用上, 其实对于使用`shared_ptr`犹豫

## const 与 constexpr

这两个B玩意, 很容易给人误解性, 真的. (~~其实是语法没学好~~)

const: **常量性**
constexpr: **编译期, 值是确定可知的**

比如说这样:
```cpp
// 编译器确认可以获取a到这个值, 做了额外的工作
const int a = 3;
int main(void)
{
    int array[a];
    return 0;
}

// g++ 扩展支持VLA
int main(void)
{
    int a;
    cin >> a;
    int array[a];
    return 0;
}

// 理论上来说, 这才是保险的做法, constexpr的值在编译期可以获取
constexpr int b = 100;
int main(void)
{
    int array[b];
    return 0;
}
```

## 使用函数取代数据的好处

这一点其实在EffectiveC++中提到过.
```cpp
class socket {
private:
    constexpr int bakclog() { return NOMAXCONN; }
};
```
如果我们使用 `constexpr int backlog = NOMAXCONN;`
首先, 我们在引入它的头文件中, 就必须`int socket::backlog`声明一次, 
因为被多个文件引用, 所以headers中不生成实例

另一方面, **设计为静态成员, 我们无法判断它是否被初始化, 一定要使用初始化的对象**
非要设计为静态成员, 就得保证它并非是non-local member, 这个就得用包装来实现.

## 小型类包装

这其中涉及到这样一个类: epoll_event数据结构

```cpp
class EpollEvent {
public:
    EpollEvent(int fd, ..) : ev_.fd(fd)........
    EpollEvent(struct epoll_event ev) : ev_(ev) { ; }
    bool operator==(EpollEvent &rhev) {
        return this->ev_ == rhev.ev_;
	}
    bool operator&(unsigned EVENT) {
        return this->ev_.events & EVENT;
	}
private:
    struct epoll_event ev_;
};
```

包装小型类意义何在呢? 
1. 这样的包装是0开销的 (函数不占空间, 结构同等大小)
即`sizeof(struct epoll_event) == sizeof(class EpollEvent)`
2. 我们将类的功能划分明确, **与事件&的接口应该是这个类提供的**, 不能在`P..S.::epoll::set`提供

## 不能浪费返回值的设计

Emmmmmm, ~~据某计算机专业人士给我说~~, 返回值是相当优美的设计, ~~整天写`void`就是傻逼~~
后来想想真的就是这么一回事.

起码,代码中还是有不合理的设计.

比如: `class epoll`的设计想法
```cpp
class epoll {
public:
    int get_io_count();
    bool wait(struct epoll_event *events, int max_events, int timeout);
private:
    int epoll_fd;
    int io_count;
};
```
将wait的返回值,存储在class内部, 是很shi的设计.
epoll族函数是**线程安全**的, epoll::wait是**非线程安全**的

将返回值存储起来, 在多线程环境下, 造成了竞争状态. 本身直接使用返回值即可. 活生生将线程安全破坏
怎么说, 并发是很现实的需求, 注意考虑不情况, 不要让自己身陷囹圄

## lambda与function

在C++11之前, 一般进行函数包装, 除了函数指针, 函数对象, 还有这样两个鬼玩意
`bind1st()`, `bind2nd()` (Essential C++中使用过, 蓝瘦)

~~怎么说呢, 真的是吃了shi,才知道饭有多香~~
~~无意冒犯 C++98 及 C++03, 只是感叹不够现代化, 现在的眼光来看, 很不友好~~

C++11中增加了三个这样的设施: `lambda表达式`, `std::function`, `std::bind`

不多扯, 尽量减少`std::bind`的使用,
`std::function`某些场景必须使用, 例如: `lambda`需要递归的时候
最后, `lambda expression` 永远是 `fucntion`行为的上位替代

## emplace_back 与 make_shared

主要说的: 其实是指合理使用这些函数

empalce_back, make_shared是直接构造, 调用构造函数, 传参即可

**如果用对象构造, 我曹, 就使用了拷贝构造. 反而效率降低 !**