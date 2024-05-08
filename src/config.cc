/*
 * @Author: lvxr
 * @Date: 2024-03-23 17:50:27
 * @LastEditTime: 2024-05-08 23:20:21
 */

#include "src/config.h"

namespace sylar {

ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
    RWMutexType::ReadLock lock(GetMutex());
    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}

/**
 * @brief: 拍平YAML文件中配置项，便于查找
 */
static void ListAllMember(
    const std::string& prefix, const YAML::Node& node,
    std::list<std::pair<std::string, const YAML::Node>>& output) {
    // 配置名错误
    if (prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678") !=
        std::string::npos) {
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())
            << "Config invalid name: " << prefix << " : " << node;
        return;
    }

    output.push_back(std::make_pair(prefix, node));
    if (node.IsMap()) {
        for (auto it = node.begin(); it != node.end(); ++it) {
            ListAllMember(prefix.empty() ? it->first.Scalar()
                                         : prefix + "." + it->first.Scalar(),
                          it->second, output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node& root) {
    std::list<std::pair<std::string, const YAML::Node>> all_nodes;
    ListAllMember("", root, all_nodes);

    for (auto& it : all_nodes) {
        std::string key = it.first;
        if (key.empty()) continue;

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr v = LookupBase(key);
        if (v) {
            if (it.second.IsScalar()) {
                v->fromString(it.second.Scalar());
            } else {
                std::stringstream ss;
                ss << it.second;
                v->fromString(ss.str());
            }
        }
    }
}
}  // namespace sylar