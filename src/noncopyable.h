/*
 * @Author: lvxr
 * @Date: 2024-03-22 19:10:39
 * @LastEditTime: 2024-03-22 20:09:03
 */

#ifndef MDS_NONCOPYABLE_H
#define MDS_NONCOPYABLE_H

namespace mds {

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
}  // namespace mds

#endif