/*
 * @Author: lvxr
 * @Date: 2024-03-24 11:54:10
 * @LastEditTime: 2024-03-24 16:35:15
 * @brief 环境变量
 */
#ifndef SYLAR_ENV_H
#define SYLAR_ENV_H

#include <map>
#include <vector>

#include "mutex.h"
#include "singleton.h"

namespace sylar {
class Env {
public:
    // 读写锁
    typedef RWMutex RWMutexType;

    /**
     * @brief 初始化环境变量
     */
    bool init(int argc, char** argv);

    /**
     * @brief: 增加或修改原有环境变量
     */
    void add(const std::string& key, const std::string& val);

    /**
     * @brief: 查询是否有该环境变量
     */
    bool has(const std::string& key);

    /**
     * @brief: 删除环境变量
     */
    void del(const std::string& key);

    /**
     * @brief: 获取环境变量的值
     * @param[in]: 键
     * @param[out]: 值
     */
    std::string get(const std::string& key,
                    const std::string& default_value = "");

    /**
     * @brief: 添加帮助信息
     * @param[in] {string&} key 键
     * @param[in] {string&} desc 描述信息
     */
    void addHelp(const std::string& key, const std::string& desc);

    /**
     * @brief: 移除帮助信息
     */
    void removeHelp(const std::string& key);

    /**
     * @brief: 打印帮助信息
     */
    void printHelp();

    /**
     * @brief: 返回程序绝对路径
     */
    const std::string& getExe() const { return m_exe; }

    /**
     * @brief: 返回当前工作目录
     */
    const std::string& getCwd() const { return m_cwd; }

    /**
     * @brief: 设置系统环境变量
     */
    bool setEnv(const std::string& key, const std::string& val);

    /**
     * @brief: 获得系统环境变量
     */
    std::string getEnv(const std::string& key,
                       const std::string& default_value = "");

    // std::string getAbsolutePath(const std::string& path) const;
    // std::string getAbsoluteWorkPath(const std::string& path) const;
    // std::string getConfigPath();

private:
    // 读写锁
    RWMutexType m_mutex;
    // 环境变量表
    std::map<std::string, std::string> m_args;
    // 帮助信息
    std::vector<std::pair<std::string, std::string> > m_helps;

    // 程序名字，即文件名
    std::string m_program;
    // 程序绝对路径
    std::string m_exe;
    // 当前工作目录
    std::string m_cwd;
};

// 单例类
typedef sylar::Singleton<Env> EnvMgr;
}  // namespace sylar

#endif