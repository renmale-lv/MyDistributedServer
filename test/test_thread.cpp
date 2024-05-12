/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-13 00:26:04
 */

#include <yaml-cpp/yaml.h>

#include <vector>

#include "../src/config.h"
#include "../src/log.h"
#include "../src/thread.h"

sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

void func1() {
    while (true) {
        SYLAR_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void func2() {
    while (true) {
        SYLAR_LOG_INFO(g_logger) << "========================================";
    }
}

void test_thread() {
    // YAML::Node root = YAML::LoadFile(
    //     "/home/nanasaki/project/MyDistributedServer/bin/conf/log.yaml");
    // sylar::Config::LoadFromYaml(root);
    std::vector<sylar::Thread::ptr> thrs;
    sylar::Thread::ptr thr(new sylar::Thread(&func1, "name_1"));
    sylar::Thread::ptr thr2(new sylar::Thread(&func2, "name_2"));
    thrs.push_back(thr);
    thrs.push_back(thr2);
    for (size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
}

int count = 0;
sylar::Mutex m_mutex;

void func3() {
    for (int i = 0; i < 100000; ++i) {
        sylar::Mutex::Lock lock(m_mutex);
        ++count;
    }
}

void test_mutex() {
    std::vector<sylar::Thread::ptr> thrs;
    for (int i = 0; i < 5; ++i) {
        sylar::Thread::ptr thr(
            new sylar::Thread(&func3, "name_" + std::to_string(i + 1)));
        thrs.push_back(thr);
    }
    for (size_t i = 0; i < thrs.size(); ++i) {
        thrs[i]->join();
    }
    SYLAR_LOG_INFO(g_logger) << count;
}

int main() {
    SYLAR_LOG_INFO(g_logger) << "thread test begin";
    // test_thread();
    test_mutex();
    SYLAR_LOG_INFO(g_logger) << "thread test end";
    // SYLAR_LOG_INFO(g_logger) << count;
    return 0;
}