/*
 * @Author: lvxr
 * @Date: 2024-03-22 19:49:50
 * @LastEditTime: 2024-03-23 21:17:15
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

// /**
//  * @brief 原子锁
//  */
// class CASLock : Noncopyable {
// public:
//     // 局部锁
//     typedef ScopedLockImpl<CASLock> Lock;

//     /**
//      * @brief 构造函数
//      */
//     CASLock() { m_mutex.clear(); }

//     /**
//      * @brief 析构函数
//      */
//     ~CASLock() {}

//     /**
//      * @brief 上锁
//      */
//     void lock() {
//         while (std::atomic_flag_test_and_set_explicit(
//             &m_mutex, std::memory_order_acquire)) {
//         }
//     }

//     /**
//      * @brief 解锁
//      */
//     void unlock() {
//         /**
//          * std::atomic_flag_clear_explicit用于以指定的内存顺序清除原子标志的值
//          * memory_order_relaxed：最轻量级的内存顺序。对原子操作没有额外的同步要求，可以乱序执行，也不会引入任何额外的内存同步开销。只保证原子操作的原子性
//          * memory_order_acquire：在原子操作前保证所有后续的读操作都不会被重排到原子操作前执行。这个内存顺序用于获取操作，保证原子操作之前的读取对后续的操作可见
//          * memory_order_release：在原子操作后保证所有先前的写操作都不会被重排到原子操作后执行。这个内存顺序用于释放操作，确保原子操作之后的写入对其他线程可见
//          * memory_order_acq_rel：同时包含了memory_order_acquire和memory_order_release的内存顺序，用于兼顾获取和释放操作
//          * memory_order_seq_cst：最严格的内存顺序，所有原子操作都按顺序执行，并且对其他线程可见性进行全局同步。这是最常用的内存顺序，也是默认的内存顺序选项
//          */
//         std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
//     }

// private:
//     // 原子状态
//     volatile std::atomic_flag m_mutex;
// };

}  // namespace sylar

#endif