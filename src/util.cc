/*
 * @Author: lvxr
 * @Date: 2024-03-24 15:10:36
 * @LastEditTime: 2024-03-24 15:10:37
 */
#include "util.h"

namespace sylar {
    pid_t GetThreadId() { return syscall(SYS_gettid); }

}  // namespace sylar
