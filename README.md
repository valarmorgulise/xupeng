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
