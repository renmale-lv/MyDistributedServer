/*
 * @Author: lvxr
 * @Date: 2024-03-24 15:10:36
 * @LastEditTime: 2024-05-20 14:22:06
 */
#include "util.h"

namespace sylar {

static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

pid_t GetThreadId() { return syscall(SYS_gettid); }

uint32_t GetFiberId() { return 0; }

void Backtrace(std::vector<std::string>& bt, int size, int skip) {
    void** array = (void**)malloc((sizeof(void*) * size));

    // backtrace()函数，获取函数调用堆栈帧数据，即回溯函数调用列表。
    // 数据将放在buffer中。参数size用来指定buffer中可以保存多少个void*元素（表示相应栈帧的地址，一个返回地址）
    // 如果回溯的函数调用大于size，则size个函数调用地址被返回。为了取得全部的函数调用列表，应保证buffer和size足够大
    size_t s = ::backtrace(array, size);
    // 将从backtrace()函数获取的地址转为描述这些地址的字符串数组
    // 每个地址的字符串信息包含对应函数的名字、在函数内的十六进制偏移地址、以及实际的返回地址（十六进制）
    char** strings = backtrace_symbols(array, s);
    if (strings == NULL) {
        SYLAR_LOG_ERROR(g_logger) << "backtrace_synbols error!";
        free(array);
        return;
    }

    for (size_t i = skip; i < s; ++i) {
        bt.push_back(strings[i]);
    }

    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix) {
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (size_t i = 0; i < bt.size(); ++i) {
        ss << prefix << bt[i] << std::endl;
    }
    return ss.str();
}

}  // namespace sylar
