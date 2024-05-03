/*
 * @Author: lvxr
 * @Date: 2024-03-24 12:20:13
 * @LastEditTime: 2024-05-03 21:55:43
 */
#ifndef SYLAR_UTIL_H
#define SYLAR_UTIL_H

#include <cxxabi.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace sylar {
/**
 * @brief 返回当前线程的ID
 */
pid_t GetThreadId();

/**
 * @brief: 返回当前协程ID 
 */
uint32_t GetFiberId();

}  // namespace sylar
#endif