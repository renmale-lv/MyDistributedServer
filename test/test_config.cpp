/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-05 00:28:24
 */
#include <yaml-cpp/yaml.h>

#include "../src/config.h"
#include "../src/log.h"

sylar::ConfigVar<int>::ptr int_value =
    sylar::Config::Lookup("aaa", (int)37, "test_int");

sylar::ConfigVar<float>::ptr float_value =
    sylar::Config::Lookup("bbb", (float)10.2f, "test_float");

void print_yaml(const YAML::Node& node, int level) {
    if (node.IsScalar()) {
        // 单个值
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
            << std::string(level * 4, ' ') << node.Scalar() << " - "
            << node.Type();
    } else if (node.IsNull()) {
        // 空值
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "NULL - " << node.Type();
    } else if (node.IsMap()) {
        // map
        for (auto it = node.begin(); it != node.end(); ++it) {
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
                << std::string(level * 4, ' ') << it->first << " - "
                << it->second.Type();
            print_yaml(it->second, level + 1);
        }
    } else if (node.IsSequence()) {
        // 数组
        for (size_t i = 0; i < node.size(); ++i) {
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
                << std::string(level * 4, ' ') << i << " - " << node[i].Type();
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml() {
    YAML::Node root = YAML::LoadFile(
        "/home/nanasaki/project/MyDistributedServer/test/log.yaml");
    // SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << root;
    print_yaml(root, 0);
}

int main() {
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << int_value->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << float_value->toString();
    test_yaml();
    return 0;
}