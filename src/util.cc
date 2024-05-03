/*
 * @Author: lvxr
 * @Date: 2024-03-24 15:10:36
 * @LastEditTime: 2024-05-03 21:55:48
 */
#include "util.h"

namespace sylar {

pid_t GetThreadId() { return syscall(SYS_gettid); }

uint32_t GetFiberId() { return 0; }

}  // namespace sylar
