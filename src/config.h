/*
 * @Author: lvxr
 * @Date: 2024-03-23 17:50:22
 * @LastEditTime: 2024-05-10 15:38:19
 */
#ifndef SYLAR_CONFIG_H
#define SYLAR_CONFIG_H

#include <yaml-cpp/yaml.h>

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "log.h"

namespace sylar {

/**
 * @brief: 配置变量的基类,纯虚类
 */
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;

    /**
     * @brief 构造函数
     * @param[in] name 配置参数名称[0-9a-z_.]
     * @param[in] description 配置参数描述
     */
    ConfigVarBase(const std::string& name, const std::string& description = "")
        : m_name(name), m_description(description) {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }

    /**
     * @brief 析构函数
     */
    virtual ~ConfigVarBase() {}

    /**
     * @brief 返回配置参数名称
     */
    const std::string& getName() const { return m_name; }

    /**
     * @brief 返回配置参数的描述
     */
    const std::string& getDescription() const { return m_description; }

    /**
     * @brief 转成字符串
     */
    virtual std::string toString() = 0;

    /**
     * @brief 从字符串初始化值
     */
    virtual bool fromString(const std::string& val) = 0;

    /**
     * @brief 返回配置参数值的类型名称
     */
    virtual std::string getTypeName() const = 0;

protected:
    // 配置参数的名称
    std::string m_name;
    // 配置参数的描述
    std::string m_description;
};

/**
 * @brief 类型转换模板类(F 源类型, T 目标类型)
 * @details
 * lexical_cast库进行"字面值"的转换，类似C中的atoi()函数，可以进行字符串与整数/浮点数之间的字面转换
 */
template <class F, class T>
class LexicalCast {
public:
    /**
     * @brief 类型转换
     * @param[in] v 源类型值
     * @return 返回v转换后的目标类型
     * @exception 当类型不可转换时抛出异常
     */
    T operator()(const F& v) { return boost::lexical_cast<T>(v); }
};

// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓模板特化↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓

template <class T>
class LexicalCast<std::string, std::vector<T>> {
public:
    std::vector<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::list<T>> {
public:
    std::list<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::set<T>> {
public:
    std::set<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::unordered_set<T>> {
public:
    std::unordered_set<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator()(const std::unordered_set<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::map<std::string, T>> {
public:
    std::map<std::string, T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin(); it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator()(const std::map<std::string, T>& v) {
        YAML::Node node(YAML::NodeType::Map);
        for (auto& i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template <class T>
class LexicalCast<std::string, std::unordered_map<std::string, T>> {
public:
    std::unordered_map<std::string, T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for (auto it = node.begin(); it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

template <class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, T>& v) {
        YAML::Node node(YAML::NodeType::Map);
        for (auto& i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

// ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑模板特化↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑

/**
 * @brief 配置参数模板子类,保存对应类型的参数值
 * @details T 参数的具体类型
 *          FromStr 从std::string转换成T类型的仿函数
 *          ToStr 从T转换成std::string的仿函数
 *          std::string 为YAML格式的字符串
 */
template <class T, class FromStr = LexicalCast<std::string, T>,
          class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef RWMutex RWMutexType;
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void(const T& old_value, const T& new_value)>
        on_change_cb;

    /**
     * @brief: 通过参数名,参数值,描述 构造ConfigVar
     */
    ConfigVar(const std::string& name, const T& default_value,
              const std::string& description = "")
        : ConfigVarBase(name, description), m_val(default_value) {}

    /**
     * @brief: 将参数值转换成string
     * @exception 转换失败抛出异常
     */
    std::string toString() override {
        try {
            RWMutexType::ReadLock lock(m_mutex);
            return ToStr()(m_val);
        } catch (std::exception& e) {
            // 输出错误信息
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())
                << "ConfigVar::toString exception " << e.what()
                << " convert: " << TypeToName<T>()
                << " to string name=" << m_name;
        }
        return "";
    }

    /**
     * @brief 从string转成参数的值
     * @exception 当转换失败抛出异常
     */
    bool fromString(const std::string& val) override {
        try {
            setValue(FromStr()(val));
        } catch (std::exception& e) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())
                << "ConfigVar::fromString exception " << e.what()
                << " convert: string to " << TypeToName<T>()
                << " name=" << m_name << " - " << val;
        }
        return false;
    }

    /**
     * @brief: 获取当前参数的值
     */
    const T getValue() {
        // 上读锁
        RWMutexType::ReadLock lock(m_mutex);
        return m_val;
    }

    /**
     * @brief: 设置当前参数的值
     * @details 如果参数的值有发生变化,则通知所有注册回调函数
     */
    void setValue(const T& v) {
        {
            // 上读锁
            RWMutexType::ReadLock lock(m_mutex);
            if (v == m_val) {
                // 如果新值和旧值相同，则不做任何操作
                return;
            }
            // 不同执行回调函数
            for (auto& it : m_cbs) {
                it.second(m_val, v);
            }
        }
        // 不相同写值，先上写锁
        RWMutexType::WriteLock lock(m_mutex);
        m_val = v;
    }

    /**
     * @brief: 返回函数类型，虚函数继承自父类
     */
    std::string getTypeName() const override { return TypeToName<T>(); }

    /**
     * @brief: 添加变换回调函数
     * @return: 返回该回调函数对应的唯一id,用于删除回调
     */
    uint64_t addListener(on_change_cb cb) {
        // 用于生成唯一id
        static uint64_t s_fun_id = 0;
        RWMutexType::WriteLock lock(m_mutex);
        ++s_fun_id;
        m_cbs[s_fun_id] = cb;
        return s_fun_id;
    }

    /**
     * @brief 删除回调函数
     * @param[in] id 回调函数的唯一id
     */
    void delListener(uint64_t id) {
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.erase(cb_id);
    }

    /**
     * @brief 获取回调函数
     * @param[in] id 回调函数的唯一id
     * @return 如果存在返回对应的回调函数,否则返回nullptr
     */
    on_change_cb getListener(uint64_t id) {
        RWMutexType::ReadLock(m_mutex);
        auto it = m_cbs.find(id);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    /**
     * @brief 清理所有的回调函数
     */
    void clearListener() {
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.clear();
    }

private:
    // 读写锁
    RWMutexType m_mutex;
    // 参数值
    T m_val;
    // 回调函数集合
    std::map<uint64_t, on_change_cb> m_cbs;
};

class Config {
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
    typedef RWMutex RWMutexType;

    /**
     * @brief 获取/创建对应参数名的配置参数
     * @param[in] name 配置参数名称
     * @param[in] default_value 参数默认值
     * @param[in] description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.]抛出异常std::invalid_argument
     */
    template <class T>
    static typename ConfigVar<T>::ptr Lookup(
        const std::string& name, const T& default_value,
        const std::string& decription = "") {
        // 上写锁
        RWMutexType::WriteLock lock(GetMutex());
        // 查找参数名称为name的参数
        auto it = GetDatas().find(name);
        // 如果存在该变量
        if (it != GetDatas().end()) {
            // 为了解决智能指针的类型转换问题，增加了std::dynamic_pointer_cast方法，该方法只适用于std::shared_ptr
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
            if (tmp) {
                // 可以转换，格式正确
                SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
                    << "Lookup name=" << name << " exists";
                return tmp;
            } else {
                // 格式不正确
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())
                    << "Lookup name=" << name << " exist but type not "
                    << TypeToName<T>()
                    << " real_type=" << it->second->getTypeName()
                    << " and value is " << it->second->toString();
                return nullptr;
            }
        }

        // 不存在该变量
        // 判断变量名字是否合法
        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._012345678") !=
            std::string::npos) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        // 创建变量
        typename ConfigVar<T>::ptr v(
            new ConfigVar<T>(name, default_value, decription));
        GetDatas()[name] = v;
        return v;
    }

    /**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        RWMutexType::ReadLock lock(GetMutex());
        auto it = GetDatas().find(name);
        if (it == GetDatas().end()) return nullptr;
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    /**
     * @brief 使用YAML::Node初始化配置模块
     */
    static void LoadFromYaml(const YAML::Node& root);

    /**
     * @brief 查找配置参数,返回配置参数的基类
     * @param[in] name 配置参数名称
     */
    static ConfigVarBase::ptr LookupBase(const std::string& name);

private:
    /**
     * @brief 返回所有的配置项
     */
    static ConfigVarMap& GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }

    /**
     * @brief 配置项的RWMutex
     */
    static RWMutexType& GetMutex() {
        static RWMutexType s_mutex;
        return s_mutex;
    }

    // 这里使用静态变量有点类似单例中的懒汉模式
};

}  // namespace sylar

#endif