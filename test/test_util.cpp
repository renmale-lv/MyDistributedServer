/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-20 14:31:02
 */

#include <assert.h>

#include "src/log.h"
#include "src/marco.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void test_assert() {
    // SYLAR_LOG_INFO(g_logger) << "\n"
    //                          << sylar::BacktraceToString(10, 0, "nanasaki");
    SYLAR_ASSERT2(0 == 1, "nanasaki xx");
}

int main() {
    test_assert();
    return 0;
}
