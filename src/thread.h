/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-11 19:24:00
 */
#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__

#include <pthread.h>

#include <functional>
#include <memory>
#include <string>

#include "noncopyable.h"

namespace sylar {

/**
 * @brief: 线程类
 * @details：使用pthread库，因为C++11thread库中没有读写锁，并且std::thread底层也是依靠pthread，因此使用pthread自己封装
 */
class Thread : Noncopyable {
public:
    typedef std::shared_ptr<Thread> ptr;

    /**
     * @brief: 线程类构造函数
     * @param[in] {function<void()>} cb 线程执行函数
     * @param[in] {string&} name 线程名称
     */
    Thread(std::function<void()> cb, const std::string& name);

    /**
     * @brief: 线程类析构函数
     */
    ~Thread();

    /**
     * @brief: 返回线程ID
     */
    pid_t getId() const { return m_id; }

    /**
     * @brief: 返回线程名称
     */
    const std::string& getName() const { return m_name; }

    /**
     * @brief: join，等待线程执行完成
     */
    void join();

    /**
     * @brief: 获取当前线程的线程指针
     */
    static Thread* GetThis();

    /**
     * @brief: 获取当前线程的线程名称
     */
    static const std::string& GetName();

    /**
     * @brief: 设置当前线程的线程名称
     * @param[in] {string&} name 线程名称
     */
    static void SetName(const std::string& name);

private:
    /**
     * @brief: 线程执行函数
     * @param[in] {void*} arg static函数，显示传入this指针
     */
    static void* run(void* arg);

private:
    // 线程ID
    pid_t m_id = -1;
    // 底层线程支持
    pthread_t m_thread = 0;
    // 线程执行函数
    std::function<void()> m_cb;
    // 线程名称
    std::string m_name;
};
}  // namespace sylar
#endif