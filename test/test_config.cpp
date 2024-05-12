/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-11 19:35:27
 */
#include <yaml-cpp/yaml.h>

#include <vector>

#include "../src/config.h"
#include "../src/log.h"
#include "../src/thread.h"

sylar::ConfigVar<int>::ptr int_value =
    sylar::Config::Lookup("test.int", (int)8080, "system int");

// sylar::ConfigVar<float>::ptr test_type =
//     sylar::Config::Lookup("test.int", (float)9090, "test type");

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
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << root;
    print_yaml(root, 0);
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

class Person {
public:
    Person() {}
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name << " age=" << m_age << " sex=" << m_sex
           << "]";
        return ss.str();
    }

    bool operator==(const Person& oth) const {
        return m_name == oth.m_name && m_age == oth.m_age && m_sex == oth.m_sex;
    }
};

namespace sylar {

template <>
class LexicalCast<std::string, Person> {
public:
    Person operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template <>
class LexicalCast<Person, std::string> {
public:
    std::string operator()(const Person& p) {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}  // namespace sylar

sylar::ConfigVar<Person>::ptr person =
    sylar::Config::Lookup("class.person", Person(), "class person");

sylar::ConfigVar<std::map<std::string, Person>>::ptr person_map =
    sylar::Config::Lookup("class.personmap",
                          std::map<std::string, Person>{{"a", Person()}},
                          "map person");

sylar::ConfigVar<std::vector<Person>>::ptr person_vec = sylar::Config::Lookup(
    "class.personvec", std::vector<Person>{Person()}, "vector person");

void test_class() {
#define XX_PM(var, prefix)                                                    \
    {                                                                         \
        auto m = var->getValue();                                             \
        for (auto& i : m) {                                                   \
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT())                                  \
                << prefix << ": " << i.first << " - " << i.second.toString(); \
        }                                                                     \
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << prefix << ": size=" << m.size();  \
    }

#define XX_PV(var, prefix)                                                   \
    {                                                                        \
        auto m = var->getValue();                                            \
        for (auto& i : m) {                                                  \
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT())                                 \
                << prefix << ": " << i.toString();                           \
        }                                                                    \
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << prefix << ": size=" << m.size(); \
    }

    SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
        << "before: " << person->getValue().toString() << " - "
        << person->toString();
    XX_PM(person_map, "map before");
    XX_PV(person_vec, "vec before");
    YAML::Node root = YAML::LoadFile(
        "/home/nanasaki/project/MyDistributedServer/test/test.yaml");
    sylar::Config::LoadFromYaml(root);
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
        << "after: " << person->getValue().toString() << " - "
        << person->toString();
    XX_PM(person_map, "map after");
    XX_PV(person_vec, "vec after");
}

void test_listener() {
    person->addListener([](const Person& old_value, const Person& new_value) {
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
            << "old_value=" << old_value.toString()
            << " new_value=" << new_value.toString();
    });
    YAML::Node root = YAML::LoadFile(
        "/home/nanasaki/project/MyDistributedServer/test/test.yaml");
    sylar::Config::LoadFromYaml(root);
}

void test_log() {
    sylar::Logger::ptr system_log = SYLAR_LOG_NAME("system");
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;
    std::cout << sylar::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile(
        "/home/nanasaki/project/MyDistributedServer/bin/conf/log.yaml");
    sylar::Config::LoadFromYaml(root);
    std::cout << "=============" << std::endl;
    std::cout << sylar::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "=============" << std::endl;
    // std::cout << root << std::endl;
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;

    system_log->setFormatter("%d - %m%n");
    SYLAR_LOG_INFO(system_log) << "hello system" << std::endl;
}

int main() {
    // test_yaml();
    // test_config();
    // test_class();
    // test_listener();
    test_log();
    return 0;
}