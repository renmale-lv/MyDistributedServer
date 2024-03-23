/*
 * @Author: lvxr
 * @Date: 2024-03-22 18:54:28
 * @LastEditTime: 2024-03-23 20:35:04
 */
#ifndef SYLAR_SINGLETON_H
#define SYLAR_SINGLETON_H

#include <memory>

namespace sylar {

/**
 * @description: 单例模式封装类
 */
template <class T>
class Singleton {
public:
    /**
     * @description: 返回单例指针
     */
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

/**
 * @description: 单例模式智能指针封装类
 */
template <class T>
class SignletonPtr {
public:
    /**
     * @description: 返回单例智能指针
     */
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};

}  // namespace sylar

#endif