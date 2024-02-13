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
