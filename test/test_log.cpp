/*
 * @Author: lvxr
 * @Date: 2024-03-23 20:48:27
 * @LastEditTime: 2024-03-23 21:24:56
 */

/*g++ -std=c++11 -pthread ../src/log.h ../src/log.cc ../src/mutex.h
 * ../src/mutex.cc ../src/noncopyable.h ../src/singleton.h test_log.cpp  -o
 * testlog*/
#include <iostream>

#include "../src/log.h"

int main() {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    sylar::FileLogAppender::ptr file_appender(
        new sylar::FileLogAppender("./log.txt"));
    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);

    logger->addAppender(file_appender);

    // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0,
    // sylar::GetThreadId(), sylar::GetFiberId(), time(0))); event->getSS() <<
    // "hello sylar log"; logger->log(sylar::LogLevel::DEBUG, event);
    std::cout << "hello sylar log" << std::endl;

    SYLAR_LOG_INFO_TEST(logger) << "test macro";
    SYLAR_LOG_ERROR_TEST(logger) << "test macro error";

    SYLAR_LOG_FMT_ERROR_TEST(logger, "test macro fmt error %s", "aa");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO_TEST(l) << "xxx";
    return 0;
}