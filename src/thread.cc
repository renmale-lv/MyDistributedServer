/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-11 16:36:38
 */

#include "thread.h"

#include "log.h"

namespace sylar {

static thread_local Thread* t_thread = nullptr;
static thread_local std::string t_thread_name = "UNKONW";

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

Thread* Thread::GetThis() { return t_thread; }

const std::string& Thread::GetName() { return t_thread_name; }

void Thread::SetName(const std::string& name) {
    if (name.empty()) return;
    if (t_thread) t_thread->m_name = name;
    t_thread_name = name;
}

Thread::Thread(std::function<void()> cb, const std::string& name)
    : m_cb(cb), m_name(name) {
    if (name.empty()) m_name = "UNKONW";
    int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    if (rt) {
        SYLAR_LOG_ERROR(g_logger)
            << "pthread_create thread fail, rt=" << rt << " name=" << name;
        throw std::logic_error("pthread_create error");
    }
}

Thread::~Thread() {
    // 分离线程
    if (m_thread) pthread_detach(m_thread);
}

void Thread::join() {
    if (m_thread) {
        int rt = pthread_join(m_thread, nullptr);
        if (rt) {
            SYLAR_LOG_ERROR(g_logger) << "pthread_join thread fail, rt = " << rt
                                      << " name=" << m_name;
            throw std::logic_error("pthread_join error");
        }
        // pthread_join会阻塞至线程执行完毕，并且回收线程资源，置0表示该线程已执行完毕
        m_thread = 0;
    }
}

void* Thread::run(void* arg) {
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = sylar::GetThreadId();
    pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());

    std::function<void()> cb;
    // 转换所有权？避免竞态条件
    cb.swap(thread->m_cb);

    cb();
    return 0;
}

}  // namespace sylar