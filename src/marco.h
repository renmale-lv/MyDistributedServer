/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-20 14:39:20
 */

#ifndef __SYLAR_MARCO_H__
#define __SYLAR_MARCO_H__

#include <assert.h>
#include <string.h>

#include "log.h"
#include "util.h"

// 减少重新取指的次数，优化
#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#    define SYLAR_LIKELY(x) __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#    define SYLAR_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#    define SYLAR_LIKELY(x) (x)
#    define SYLAR_UNLIKELY(x) (x)
#endif

// 断言宏封装
#define SYLAR_ASSERT(x)                                  \
    if (SYLAR_UNLIKELY(!(x))) {                          \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())                \
            << "ASSERTION: " #x << "\nbacktrace:\n"      \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x);                                       \
    }

// 断言宏封装
#define SYLAR_ASSERT2(x, w)                              \
    if (SYLAR_UNLIKELY(!(x))) {                          \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())                \
            << "ASSERTION: " #x << "\n"                  \
            << w << "\nbacktrace:\n"                     \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x);                                       \
    }

#endif