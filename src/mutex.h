/*
 * @Author: lvxr
 * @Date: 2024-03-22 19:49:50
 * @LastEditTime: 2024-05-12 23:59:20
 */

#ifndef SYLAR_MUTEX_H
#define SYLAR_MUTEX_H

#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

#include <atomic>
#include <functional>
#include <list>
#include <memory>
#include <thread>

#include "noncopyable.h"

namespace sylar {

/**
 * @brief 信号量
 */
class Semaphore : Noncopyable {
public:
    /**
     * @brief 构造函数
     * @param[in] count 信号量值的大小
     */
    Semaphore(uint32_t count = 0);

    /**
     * @brief 析构函数
     */
    ~Semaphore();

    /**
     * @brief 获取信号量
     */
    void wait();

    /**
     * @brief 释放信号量
     */
    void notify();

private:
    // 信号量
    sem_t m_semaphore;
};

/**
 * @brief 局部锁的模板
 */
template <class T>
class ScopedLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 信号量
     */
    ScopedLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
    }

    /**
     * @brief 析构函数,自动释放锁
     */
    ~ScopedLockImpl() { unlock(); }

    /**
     * @brief 加锁
     */
    void lock() {
        if (!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    /**
     * @brief 解锁
     */
    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    // 互斥锁
    T &m_mutex;
    // 是否已经上锁
    bool m_locked;
};

/**
 * @brief 局部读锁模板
 */
template <class T>
class ReadScopedLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 读写锁
     */
    ReadScopedLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.rdlock();
        m_locked = true;
    }

    /**
     * @brief 析构函数,自动释放锁
     */
    ~ReadScopedLockImpl() { unlock(); }

    /**
     * @brief 上读锁
     */
    void lock() {
        if (!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    /**
     * @brief 释放锁
     */
    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    // 互斥锁
    T &m_mutex;
    // 是否已经上锁
    bool m_locked;
};

/**
 * @brief 局部写锁模板
 */
template <class T>
class WriteScopedLockImpl {
public:
    /**
     * @brief 构造函数
     * @param[in] mutex 读写锁
     */
    WriteScopedLockImpl(T &mutex) : m_mutex(mutex) {
        m_mutex.wrlock();
        m_locked = true;
    }

    /**
     * @brief 析构函数
     */
    ~WriteScopedLockImpl() { unlock(); }

    /**
     * @brief 上写锁
     */
    void lock() {
        if (!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    /**
     * @brief 解锁
     */
    void unlock() {
        if (m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    // 互斥锁
    T &m_mutex;
    // 是否已上锁
    bool m_locked;
};

/**
 * @brief 互斥量
 */
class Mutex : Noncopyable {
public:
    // 局部锁
    typedef ScopedLockImpl<Mutex> Lock;

    /**
     * @brief 构造函数
     */
    Mutex() { pthread_mutex_init(&m_mutex, nullptr); }

    /**
     * @brief 析构函数
     */
    ~Mutex() { pthread_mutex_destroy(&m_mutex); }

    /**
     * @brief 加锁
     */
    void lock() { pthread_mutex_lock(&m_mutex); }

    /**
     * @brief 解锁
     */
    void unlock() { pthread_mutex_unlock(&m_mutex); }

private:
    // 互斥锁
    pthread_mutex_t m_mutex;
};

/**
 * @brief 读写互斥量
 */
class RWMutex : Noncopyable {
public:
    /// 局部读锁
    typedef ReadScopedLockImpl<RWMutex> ReadLock;

    /// 局部写锁
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    /**
     * @brief 构造函数
     */
    RWMutex() { pthread_rwlock_init(&m_lock, nullptr); }

    /**
     * @brief 析构函数
     */
    ~RWMutex() { pthread_rwlock_destroy(&m_lock); }

    /**
     * @brief 上读锁
     */
    void rdlock() { pthread_rwlock_rdlock(&m_lock); }

    /**
     * @brief 上写锁
     */
    void wrlock() { pthread_rwlock_wrlock(&m_lock); }

    /**
     * @brief 解锁
     */
    void unlock() { pthread_rwlock_unlock(&m_lock); }

private:
    // 读写锁
    pthread_rwlock_t m_lock;
};

/**
 * @brief 自旋锁
 */
class Spinlock : Noncopyable {
public:
    /// 局部锁
    typedef ScopedLockImpl<Spinlock> Lock;

    /**
     * @brief 构造函数
     */
    Spinlock() { pthread_spin_init(&m_mutex, 0); }

    /**
     * @brief 析构函数
     */
    ~Spinlock() { pthread_spin_destroy(&m_mutex); }

    /**
     * @brief 上锁
     */
    void lock() { pthread_spin_lock(&m_mutex); }

    /**
     * @brief 解锁
     */
    void unlock() { pthread_spin_unlock(&m_mutex); }

private:
    // 自旋锁
    pthread_spinlock_t m_mutex;
};

}  // namespace sylar

#endif