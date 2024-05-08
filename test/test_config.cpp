/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-08 23:14:58
 */
#include <yaml-cpp/yaml.h>

#include <vector>

#include "../src/config.h"
#include "../src/log.h"

sylar::ConfigVar<int>::ptr int_value =
    sylar::Config::Lookup("test.int", (int)8080, "system int");

sylar::ConfigVar<float>::ptr float_value =
    sylar::Config::Lookup("test.float", (float)10.2f, "system float");

sylar::ConfigVar<std::vector<int>>::ptr vector_value = sylar::Config::Lookup(
    "test.vector", std::vector<int>{1, 2}, "system vector");

sylar::ConfigVar<std::list<int>>::ptr list_value =
    sylar::Config::Lookup("test.list", std::list<int>{1, 2}, "system list");

sylar::ConfigVar<std::set<int>>::ptr set_value =
    sylar::Config::Lookup("test.set", std::set<int>{1, 2}, "system set");

sylar::ConfigVar<std::unordered_set<int>>::ptr uset_value =
    sylar::Config::Lookup("test.uset", std::unordered_set<int>{1, 2},
                          "system uset");

sylar::ConfigVar<std::map<std::string, int>>::ptr map_value =
    sylar::Config::Lookup("test.map", std::map<std::string, int>{{"a", 1}},
                          "system map");

sylar::ConfigVar<std::unordered_map<std::string, int>>::ptr umap_value =
    sylar::Config::Lookup("test.umap",
                          std::unordered_map<std::string, int>{{"a", 1}},
                          "system umap");

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
        "/home/nanasaki/project/MyDistributedServer/test/test.yaml");
    // SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << root;
    // print_yaml(root, 0);
    sylar::Config::LoadFromYaml(root);
}

void test_config() {
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "befort int: " << int_value->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
        << "befort float: " << float_value->toString();

#define XX(var, name, prefix)                                                 \
    {                                                                         \
        auto& v = var->getValue();                                            \
        for (auto& it : v) {                                                  \
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name ": " << it; \
        }                                                                     \
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT())                                      \
            << #prefix " " #name " yaml: " << var->toString();                \
    }

#define XX_M(g_var, name, prefix)                                          \
    {                                                                      \
        auto& v = g_var->getValue();                                       \
        for (auto& i : v) {                                                \
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT())                               \
                << #prefix " " #name ": {" << i.first << " - " << i.second \
                << "}";                                                    \
        }                                                                  \
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT())                                   \
            << #prefix " " #name " yaml: " << g_var->toString();           \
    }

    XX(vector_value, vector, before);
    XX(list_value, list, before);
    XX(set_value, set, before);
    XX(uset_value, uset, before);
    XX_M(map_value, map, before);
    XX_M(umap_value, umap, before);
    YAML::Node root = YAML::LoadFile(
        "/home/nanasaki/project/MyDistributedServer/test/test.yaml");
    sylar::Config::LoadFromYaml(root);

    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after int: " << int_value->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
        << "after float: " << float_value->toString();

    XX(vector_value, vector, after);
    XX(list_value, list, after);
    XX(set_value, set, after);
    XX(uset_value, uset, after);
    XX_M(map_value, map, after);
    XX_M(umap_value, umap, after);
}

int main() {
    test_config();
    return 0;
}