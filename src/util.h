/*
 * @Author: lvxr
 * @Date: 2024-03-24 12:20:13
 * @LastEditTime: 2024-05-09 17:05:52
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

/**
 * @brief: 萃取，用于在模板中获取函数类型
 */
template <class T>
const char* TypeToName() {
    // abi::__cxa_demangle 是 C++ ABI (Application Binary Interface)
    // 中的一个函数，用于将编译器产生的 C++ 符号名还原为人类可读的形式。在 C++
    // 中，函数名、类名等在编译过程中会被编译器改写为一种编码形式，这种形式不易阅读，而
    // abi::__cxa_demangle可以将其还原为正常的形式，便于理解和调试。具体来说，abi::__cxa_demangle
    // 函数的作用是将 C++ 符号名（mangled name）转换为其原始形式
    static const char* s_name =
        abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return s_name;
}

}  // namespace sylar
#endif