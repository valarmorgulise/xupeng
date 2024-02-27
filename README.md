# xupeng

## hello world

配置的事件机制
当一个配置项发生修改的时候，可以反向通知对应的代码

# 日志系统整合配置系统
```yaml
logs：
    - name: root
      level: (debug, info, warn, error, fatal)
      formatter: '%d%T%p%T%t%m%n'
      appender:
        - type: (StdoutLogAppender, FileLogAppender)
          level:(debug...)
          file: /logs/xxx.log
```

```cpp
    PENG::Logger g_logger = PENG::LoggerMgr::GetInstance()->getLogger(anme);
    PENG_LOG_INFO(g_logger) << "xxxx log";
```

```cpp
static Logger::ptr g_log = PENG_LOG_NAME("system");
//m_root, m_system->m_root 当logger的appenders为空， 使用root写logger

```
# 线程库
Thread， Mutex
Pthread

pthread pthread_create

互斥量 Mutex
信号量 semaphore

和log整合
Logger, appender

Spinlock， 替换Mutex
写文件， 周期性， reopen

# 协程库
定义协程接口
ucontext_t 
macro

```
Thread->main_fiber <-------->sub_fiber
            ^
            |
            |
            v
        sub_fiber
```

怎么能让协程在线程切换
协程调度模块scheduler
```
线程池概念，可以分配多个线程
 1 - n                     1-M 
scheduler   --> thread    ---> fiber
1.线程池，分配一组线程
2.协程调度器， 将协程，指定到相应的线程上执行

N : M

m_threads
<function<void()>, fiber, thread_id> m_ifibers

schedule(func/fiber)

start()
stop()
run() 协程调度模块核心

```

#io 协程调度

```
IOManager (epoll)  --> Scheduler
    |
    |
    v  
idle (epoll_wait)
```

#IO调度器

```
IOManager (epoll)  ---> Scheduler
    |
    |
    V 
    idle (epoll_wait)

    信号量
PutMessage(msg, ) +信号量1, single();

message_queue
    |
    | ------ Thread
    | ------ Thread
        wait()-信号量1, RecvMessage(msg,)

异步IO, 等待数据返回，在epoll_wait等待
文件句柄

```

#Hook
##概述
1. hook模块 hook底层和socket相关的API、socket IO相关的API、sleep相关的API。
2. hook的开启控制是线程粒度的，可以自由选择（通过线程局部变量实现）。
3. 通过hook模块，可以使一些不具异步功能的API，展现出异步的性能，如MySQL。实现上是通过定时器、协程、IO协程调度等模块的配合。
4. hook其实是对系统调用API进行一次封装，将其封装成一个与原始的系统调用API同名的接口，应用在调用这个接口时，会先执行封装中的操作，再执行原始的系统调用API。
5. hook 的目的是在不重新编写代码的情况下，把老代码中的 socket IO 相关的 API 都转成异步，以提高性能。即以同步的方式编写代码，实现的效果却是异步执行的，效率很高。
6. 在 IO 协程调度中对相关的系统调用进行 hook，可以让调度线程尽可能得把时间片都花在有意义的操作上，而不是浪费在阻塞等待中。
7.  hook 的实现机制就是通过动态库的全局符号介入功能，用自定义的接口来替换掉同名的系统调用接口。由于系统调用接口基本上是由 C 标准函数库 libc 提供的，所以这里要做的事情就是用自定义的动态库来覆盖掉 libc 中的同名符号。
```
那些被覆盖的符号，它们只是被“雪藏”了而已，实际还是存在于程序的进程空间中的，通过一定的办法，可以把它们再找回来。在 Linux 中，这个方法就是 dslym，它的函数原型如下：
#define _GNU_SOURCE
#include <dlfcn.h>
void *dlsym(void *handle, const char *symbol);
```
```
关于 dlsym 的使用可参考 man 3 dlsym，在链接时需要指定 -ldl 参数。使用 dlsym 找回被覆盖的符号时，第一个参数固定为 RTLD_NEXT，第二个参数为符号的名称。
```
8.为了管理所有的 socket fd，设计了一个 FdManager 类来记录所有分配过的 fd 的上下文，这是一个单例类，每个 socket fd 上下文记录了当前 fd 的读写超时，是否设置非阻塞等信息。

## FdCtx
```
文件句柄上下文类。
FdCtx 类在用户态记录了 fd 的读写超时和非阻塞信息，其中非阻塞包括用户显式设置的非阻塞和 hook 内部设置的非阻塞，区分这两种非阻塞可以有效应对用户对 fd 设置/获取 NONBLOCK 模式的情形。
```

## FdManager
```
文件局部上下文管理类。
FdManager 类对 FdCtx 的寻址采用了直接用fd作为数组下标进行寻址的方式。
```

##hook
```
hook 模块。
线程局部变量 t_hook_enable，用于表示当前线程是否启用 hook，使用线程局部变量表示 hook 模块是线程粒度的，各个线程可单独启用或关闭 hook。
hook_init() 函数放在 _HookIniter 结构体里面，而 _HookIniter 类型的全局静态变量 s_hook_initer 会在 main() 函数运行之前初始化，即会在 main() 函数运行之前调用 hook_init()。
各个接口的 hook 实现要放在 extern “C” 中，以防止 C++ 编译器对符号名称添加修饰。
```
##其他说明
```
在编译时给 gcc 增加一个 “-v” 参数，可以将整个编译流程详细地打印出来。
通过设置 LD_PRELOAD 环境变量，可以设置成优先加载相关动态库。
默认情况下，协程调度器的调度线程会开启 hook，而其他线程则不会开启。
hook socket/fcntl/ioctl/close 等接口，这类接口主要处理的是边缘情况，比如分配 fd 上下文，处理超时及用户显式设置非阻塞问题。
所有参与协程调度的 fd 都会被设置成非阻塞模式，所以要在应用层维护好用户设置的非阻塞标志。
按 xupeng hook 模块的实现，非调度线程不支持启用 hook。
```
```
         [Fiber]                 [Timer]
            ^                       ^
            | N                     |
            |                       |
            | 1                     |
         [Thread]             [TimerManager]
            ^ M                     ^
            |                       |
            | 1                     |
        [Scheduler]  <-----  [IOManager(epoll)]
```

sleep, usleep,

socket 相关的（socket, connect, accept)
io相关的 （read, write, send, recv....)
fd相关的 （fcntl, ioctl, ...)

## socket函数库
                [UnixAddress]
                    |
                ----------                  |-[IPv4Address]
                 |Address| ---[IPAddress] --|
                ----------                  |-[IPv6Address]
                    |
                
                ----------
                | Socket |
                ----------
## http协议开发
HTTP/1.1 -API

HttpRequest
HttpResponse 

GET / HTTP/1.1 
host: www.sylar.top 

HTTP/1.0 200 OK
Pragma: no-cache 
Content-Type: text/html
Content-Length: 14988 
Connection: close 

uri: http://www.sylar.top:80/page/xxx? id = 10& v=20 #fr 
    http, 
    www.sylar.top, host 
    80 
    /age/xxx, path
    id = 10&v=20 param 
    fr fragment



