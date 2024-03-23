/*
 * @Author: lvxr
 * @Date: 2024-03-22 19:49:56
 * @LastEditTime: 2024-03-22 20:55:05
 */

// 最好不要在头文件中throw异常

#include "mutex.h"

namespace mds {
Semaphore::Semaphore(uint32_t count) {
    if (sem_init(&m_semaphore, 0, count)) {
        throw std::logic_error("sem_init error");
    }
}

Semaphore::~Semaphore() { sem_destroy(&m_semaphore); }

void Semaphore::wait() {
    if (sem_wait(&m_semaphore)) {
        throw std::logic_error("sem_wait error");
    }
}

void Semaphore::notify() {
    if (sem_post(&m_semaphore)) {
        throw std::logic_error("sem_post error");
    }
}
}  // namespace mds