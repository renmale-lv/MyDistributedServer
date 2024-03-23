/*
 * @Author: lvxr
 * @Date: 2024-03-22 18:48:26
 * @LastEditTime: 2024-03-23 21:06:24
 */
#ifndef SYLAR_LOG_H
#define SYLAR_LOG_H

#include <stdarg.h>
#include <stdint.h>

#include <fstream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "mutex.h"
#include "singleton.h"
/**
 * 流程：
 * 1.初始化LogFormatter，LogAppender, Logger
 * 2.通过宏定义提供流式风格和格式化风格的日志接口。每次写日志时，通过宏自动生成对应的日志事件LogEvent，并且将日志事件和日志器Logger包装到一起，生成一个LogEventWrap对象
 * 3.日志接口执行结束后，LogEventWrap对象析构，在析构函数里调用Logger的log方法将日志事件进行输出
 */

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 * @details 返回一个输入流
 * @todo 启动依赖耗时未实现
 */
#define SYLAR_LOG_LEVEL(logger, level)                                  \
    if (logger->getLevel() <= level)                                    \
    sylar::LogEventWrap(                                                \
        sylar::LogEvent::ptr(new sylar::LogEvent(                       \
            logger, level, __FILE__, __LINE__, 0, sylar::GetThreadId(), \
            sylar::GetFiberId(), time(0), sylar::Thread::GetName())))   \
        .getSS()

/**
 * @brief 测试用
 */
#define SYLAR_LOG_LEVEL_TEST(logger, level)                                    \
    if (logger->getLevel() <= level)                                           \
    sylar::LogEventWrap(                                                       \
        sylar::LogEvent::ptr(new sylar::LogEvent(                              \
            logger, level, __FILE__, __LINE__, 0, 0, 0, time(0), "log_test"))) \
        .getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)
#define SYLAR_LOG_DEBUG_TEST(logger) \
    SYLAR_LOG_LEVEL_TEST(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)
#define SYLAR_LOG_INFO_TEST(logger) \
    SYLAR_LOG_LEVEL_TEST(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)
#define SYLAR_LOG_WARN_TEST(logger) \
    SYLAR_LOG_LEVEL_TEST(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)
#define SYLAR_LOG_ERROR_TEST(logger) \
    SYLAR_LOG_LEVEL_TEST(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)
#define SYLAR_LOG_FATAL_TEST(logger) \
    SYLAR_LOG_LEVEL_TEST(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...)                    \
    if (logger->getLevel() <= level)                                    \
    sylar::LogEventWrap(                                                \
        sylar::LogEvent::ptr(new sylar::LogEvent(                       \
            logger, level, __FILE__, __LINE__, 0, sylar::GetThreadId(), \
            sylar::GetFiberId(), time(0), sylar::Thread::GetName())))   \
        .getEvent()                                                     \
        ->format(fmt, __VA_ARGS__)

/**
 * @brief 测试用
 */
#define SYLAR_LOG_FMT_LEVEL_TEST(logger, level, fmt, ...)                      \
    if (logger->getLevel() <= level)                                           \
    sylar::LogEventWrap(                                                       \
        sylar::LogEvent::ptr(new sylar::LogEvent(                              \
            logger, level, __FILE__, __LINE__, 0, 0, 0, time(0), "log_test"))) \
        .getEvent()                                                            \
        ->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_DEBUG_TEST(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL_TEST(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_INFO_TEST(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL_TEST(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_WARN_TEST(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL_TEST(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_ERROR_TEST(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL_TEST(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_FATAL_TEST(logger, fmt, ...) \
    SYLAR_LOG_FMT_LEVEL_TEST(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define SYLAR_LOG_ROOT() sylar::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief 获取name的日志器
 */
#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)

namespace sylar {
class Logger;
class LoggerManager;

/**
 * @brief: 日志级别
 */
class LogLevel {
public:
    // 日志级别
    enum Level {
        // 未设置
        UNKNOW = 0,
        // 调试信息
        DEBUG = 1,
        // 一般信息
        INFO = 2,
        // 警告
        WARN = 3,
        // 错误
        ERROR = 4,
        // 致命情况
        FATAL = 5
    };

    // 将日志级别转换成文本输出
    static const char* ToString(LogLevel::Level level);

    // 将文本装换成日志级别
    static LogLevel::Level FromString(const std::string& str);
};

/**
 * @brief 日志事件类，用于记录日志现场
 * @details 一行日志对应一个日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;

    /**
     * @brief 构造函数
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(秒)
     * @param[in] thread_name 线程名称
     */
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
             const char* file, int32_t line, uint32_t elapse,
             uint32_t thread_id, uint32_t fiber_id, uint64_t time,
             const std::string& thread_name);

    /**
     * @brief 返回文件名
     */
    const char* getFile() const { return m_file; }

    /**
     * @brief 返回行号
     */
    int32_t getLine() const { return m_line; }

    /**
     * @brief 返回耗时
     */
    uint32_t getElapse() const { return m_elapse; }

    /**
     * @brief 返回线程ID
     */
    uint32_t getThreadId() const { return m_threadId; }

    /**
     * @brief 返回协程ID
     */
    uint32_t getFiberId() const { return m_fiberId; }

    /**
     * @brief 返回时间
     */
    uint64_t getTime() const { return m_time; }

    /**
     * @brief 返回线程名称
     */
    const std::string& getThreadName() const { return m_threadName; }

    /**
     * @brief 返回日志内容
     */
    std::string getContent() const { return m_ss.str(); }

    /**
     * @brief 返回日志器
     */
    std::shared_ptr<Logger> getLogger() const { return m_logger; }

    /**
     * @brief 返回日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 返回日志内容字符串流
     */
    std::stringstream& getSS() { return m_ss; }

    /**
     * @brief 格式化写入日志内容
     */
    void format(const char* fmt, ...);

    /**
     * @brief 格式化写入日志内容
     */
    void format(const char* fmt, va_list al);

private:
    // 文件名
    const char* m_file = nullptr;
    // 行号
    int32_t m_line = 0;
    // 程序启动到现在的毫秒数
    uint32_t m_elapse = 0;
    // 线程ID
    uint32_t m_threadId = 0;
    // 协程ID
    uint32_t m_fiberId = 0;
    // 时间戳
    uint64_t m_time = 0;
    // 线程名称
    std::string m_threadName;
    // 日志内容流
    std::stringstream m_ss;
    // 日志器
    std::shared_ptr<Logger> m_logger;
    // 日志等级
    LogLevel::Level m_level;
};

/**
 * @brief 日志事件包装器
 * @details
 * 在构建时指定日志事件，日志事件内包含日志器，在析构时会调用日志器输出日志信息
 */
class LogEventWrap {
public:
    /**
     * @brief 构造函数
     * @param[in] e 日志事件
     */
    LogEventWrap(LogEvent::ptr e);

    /**
     * @brief 析构函数
     */
    ~LogEventWrap();

    /**
     * @brief 获取日志事件
     */
    LogEvent::ptr getEvent() const { return m_event; }

    /**
     * @brief 获取日志内容流
     */
    std::stringstream& getSS();

private:
    /**
     * @brief 日志事件
     */
    LogEvent::ptr m_event;
};

/**
 * @brief 日志格式化
 */
class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    /**
     * @brief 构造函数
     * @param[in] pattern 格式模板
     * @details
     *  %m 消息
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *  %N 线程名称
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    LogFormatter(const std::string& pattern);

    /**
     * @brief 返回格式化日志文本
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                       LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger,
                         LogLevel::Level level, LogEvent::ptr event);

public:
    /**
     * @brief 日志内容项格式化
     * @details 通过继承该类实现某一个格式的格式化
     */
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        /**
         * @brief 析构函数
         */
        virtual ~FormatItem() {}

        /**
         * @brief 格式化日志到流
         * @param[in, out] os 日志输出流
         * @param[in] logger 日志器
         * @param[in] level 日志等级
         * @param[in] event 日志事件
         */
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger,
                            LogLevel::Level level, LogEvent::ptr event) = 0;
    };

    /**
     * @brief 初始化,解析日志模板
     */
    void init();

    /**
     * @brief 是否有错误
     */
    bool isError() const { return m_error; }

    /**
     * @brief 返回日志模板
     */
    const std::string getPattern() const { return m_pattern; }

private:
    // 日志格式模板
    std::string m_pattern;
    // 日志格式解析后格式
    std::vector<FormatItem::ptr> m_items;
    // 是否有错误
    bool m_error = false;
};

/**
 * @brief 日志输出器
 * @details
 * 内部包含一个LogFormatter成员和一个log方法，日志事件先经过LogFormatter格式化后再输出到对应的输出地
 * 派生出不同的Appender类型，比如StdoutLogAppender和FileLogAppender，分别表示输出到终端和文件
 */
class LogAppender {
    friend class Logger;

public:
    typedef std::shared_ptr<LogAppender> ptr;
    // 自旋锁
    typedef Spinlock MutexType;

    /**
     * @brief 析构函数
     */
    virtual ~LogAppender() {}

    /**
     * @brief 写入日志
     * @param[in] logger 日志器
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     */
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     LogEvent::ptr event) = 0;

    // /**
    //  * @brief 将日志输出目标的配置转成YAML String
    //  */
    // virtual std::string toYamlString() = 0;

    /**
     * @brief 更改日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     * @brief 获取日志格式器
     */
    LogFormatter::ptr getFormatter();

    /**
     * @brief 获取日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 设置日志级别
     */
    void setLevel(LogLevel::Level val) { m_level = val; }

protected:
    // 日志级别
    LogLevel::Level m_level = LogLevel::DEBUG;
    // 是否有自己的日志格式器
    bool m_hasFormatter = false;
    // Mutex
    MutexType m_mutex;
    // 日志格式器
    LogFormatter::ptr m_formatter;
};

/**
 * @brief 日志器，复制进行日志输出
 * @details
 * 一个Logger包含多个LogAppender和一个日志级别，提供log方法
 * 传入日志事件，判断该日志事件的级别高于日志器本身的级别之后调用LogAppender将日志进行输出，否则该日志被抛弃
 */
class Logger : public std::enable_shared_from_this<Logger> {
    // enable_shared_from_this是方便在类内部获得一个指向自身的shared_ptr

    friend class LoggerManager;

public:
    typedef std::shared_ptr<Logger> ptr;
    // 自旋锁
    typedef Spinlock MutexType;

    /**
     * @brief 构造函数
     * @param[in] name 日志器名称
     */
    Logger(const std::string& name = "root");

    /**
     * @brief 写日志
     * @param[in] level 日志级别
     * @param[in] event 日志事件
     * @details 向所有目标集合中写日志，如果为空则通知主日志器
     */
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     * @brief 写debug级别日志
     * @param[in] event 日志事件
     */
    void debug(LogEvent::ptr event);

    /**
     * @brief 写info级别日志
     * @param[in] event 日志事件
     */
    void info(LogEvent::ptr event);

    /**
     * @brief 写warn级别日志
     * @param[in] event 日志事件
     */
    void warn(LogEvent::ptr event);

    /**
     * @brief 写error级别日志
     * @param[in] event 日志事件
     */
    void error(LogEvent::ptr event);

    /**
     * @brief 写fatal级别日志
     * @param[in] event 日志事件
     */
    void fatal(LogEvent::ptr event);

    /**
     * @brief 添加日志目标
     * @param[in] appender 日志目标
     */
    void addAppender(LogAppender::ptr appender);

    /**
     * @brief 删除日志目标
     * @param[in] appender 日志目标
     */
    void delAppender(LogAppender::ptr appender);

    /**
     * @brief 清空日志目标
     */
    void clearAppenders();

    /**
     * @brief 返回日志级别
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 设置日志级别
     */
    void setLevel(LogLevel::Level val) { m_level = val; }

    /**
     * @brief 返回日志名称
     */
    const std::string& getName() const { return m_name; }

    /**
     * @brief 设置日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     * @brief 设置日志格式模板
     */
    void setFormatter(const std::string& val);

    /**
     * @brief 获取日志格式器
     */
    LogFormatter::ptr getFormatter();

    /**
     * @brief 将日志器的配置转成YAML String
     */
    std::string toYamlString();

private:
    // 日志名称
    std::string m_name;
    // 日志级别，大于该级别的日志才会被打印
    LogLevel::Level m_level;
    // Mutex，互斥访下面三个元素
    MutexType m_mutex;
    // 日志目标集合
    std::list<LogAppender::ptr> m_appenders;
    // 日志格式器
    LogFormatter::ptr m_formatter;
    // 主日志器
    Logger::ptr m_root;
};

/**
 * @brief 输出到控制台的Appender
 */
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level,
             LogEvent::ptr event) override;
    // std::string toYamlString() override;
};

/**
 * @brief 输出到文件的Appender
 */
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(Logger::ptr logger, LogLevel::Level level,
             LogEvent::ptr event) override;
    // std::string toYamlString() override;

    /**
     * @brief 重新打开日志文件
     * @return 成功返回true
     */
    bool reopen();

private:
    // 文件路径
    std::string m_filename;
    // 文件流
    std::ofstream m_filestream;
    // 上次重新打开时间
    uint64_t m_lastTime = 0;
};

/**
 * @brief 日志器管理类
 * @details 统一管理所有的日志器，自带一个root日志器
 */
class LoggerManager {
public:
    typedef Spinlock MutexType;
    /**
     * @brief 构造函数
     */
    LoggerManager();

    /**
     * @brief 获取日志器
     * @param[in] name 日志器名称
     * @details 根据名字返回一个日志器，找不到则新建一个
     */
    Logger::ptr getLogger(const std::string& name);

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 返回主日志器
     */
    Logger::ptr getRoot() const { return m_root; }

    /**
     * @brief 将所有的日志器配置转成YAML String
     */
    std::string toYamlString();

private:
    // Mutex
    MutexType m_mutex;
    // 日志器容器
    std::map<std::string, Logger::ptr> m_loggers;
    // 主日志器
    Logger::ptr m_root;
};

// 日志器管理类单例模式
typedef sylar::Singleton<LoggerManager> LoggerMgr;

}  // namespace sylar

#endif
