/*
 * @Author: lvxr
 * @Date: 2024-03-22 19:10:39
 * @LastEditTime: 2024-03-23 21:07:21
 */

#ifndef sylar_NONCOPYABLE_H
#define sylar_NONCOPYABLE_H

namespace sylar {

/**
 * @description: 仿照muduo库，继承该类的类无法拷贝，赋值
 */
class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
}  // namespace sylar

#endif