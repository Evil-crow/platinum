![](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/logo.png)

## Table of content

* [Overview](#Overview)
* [Features](#Features)
* [Performance](#Performance)
* [Installation](#Installation)
* [Config](#Config)
* [Thanks](#Thanks)
* [Licence](#License)
* [Links](#Links)

## Overview

本项目是使用C++编写的基于事件驱动模型的Web Server. 目前支持静态GET请求以及FastCGI请求. 以及部分HTTP/1.1特性.

## Features

- 基于事件驱动模型
- Reactor风格
- C++11部分特性 (`std::thread`, `std::function` ...)
- Linux Kernel 2.6+新特性(`eventfd(2)`, `REUSEPORT`)
- 可配置的消息解析器(`HttpParser`, `FcgiParser`)
- 基于状态机的HTTP协议解析器
- 基于时间轮的定时器
- YAML配置文件

## Performance

使用工具: Webbench ver1.5

性能图表:

![Apache VS Platinum](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/apache_platinum.JPG)

![Platinum](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/platinum.JPG)

![Apache 1 client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/apache_1client.png)

![Platinum 1 client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/platinum__1client.png)

![Apache 10 Client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/apache_10client.png)

![Platinum 10 Client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/platinum__10client.png)

![Apache 3000 Client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/apche_3000client.png)

![Platinum 3000 Client](https://raw.githubusercontent.com/Evil-crow/platinum/master/img/platinum__3000client.png)

具体测试数据见相关文件 [Here](https://github.com/Evil-crow/platinum/tree/master/img)

## Installation

- cmake >= 3.0
- yaml-cpp >= 5.7
- REUSEPORT (Kernel > 2.6.23)

请在满足以上条件之后进行测试运行.

```bash
$ git clone ...
$ cd platinum
$ mkdir build
$ cd build
$ sudo cmake ..
$ sudo make && make install
```

## Config
platinum 通过指定的配置文件`/etc/platinum.yaml`进行配置.使用YAML进行配置,可以简单快速的上手.
基本配置内容如下:

```
- server:
	- port:         # 端口号
	- thread:       # 线程数目
	- method:       # 支持的方法,e.g. [GET, POST]
- log_enable:		# 日志记录 true/false
- resource:
	- static:       # 支持的静态请求文件 e.g. [html, js, css]
	- dynamic:      # 支持的动态请求文件类型 e.g. [php]
	- forbidden:    # 禁止访问的内容
	- index:        # 默认的主页
	- www-root:     # 资源文件目录
	- default-root: # 特殊文件目录 e.g.404.html, 403.html
- fcgi:
	- listen:       # 监听类型, inet/unix
	- addr:         # 监听地址,同php-fpm/www.conf 中listen配置
	- fcgi-root:    # FCGI文件配置目录
```

下面是一份简易的配置样例
```bash
// platinum.yaml Example
server:
  port: 9887
  thread: 8
  method: [GET, HEAD, POST]

log_enable: false

resource:
  static: [img, html, css, js, txt]
  dynamic: [php]
  forbidden: [/core, /config]
  index: index.html
  www-root: /var/www/html
  default-root: /var/www-root/default

fcgi:
  listen: inet
  addr: 127.0.0.1:9000
  fcgi-root: /home/Crow

```

## Thanks

特别感谢:
- [muduo](https://github.com/chenshuo/muduo): 网络层深度参考muduo进行设计
- [yaml-cpp](https://github.com/jbeder/yaml-cpp): YAML配置文件解析
- [httpxx](https://github.com/AndreLouisCaron/httpxx): HTTP协议解析器(v1.0使用,现已使用自定义解析器)

## License

本项目遵循MIT开源协议,[详细内容>>](https://github.com/Evil-crow/platinum/blob/master/LICENSE)

## Links