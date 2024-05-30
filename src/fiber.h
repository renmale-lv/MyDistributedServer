/*
 * @Author: lvxr
 * @LastEditTime: 2024-05-22 16:24:49
 */

/*
上下文结构体定义
这个结构体是平台相关的，因为不同平台的寄存器不一样
下面列出的是所有平台都至少会包含的4个成员

typedef struct ucontext_t {
    //当前上下文结束后，下一个激活的上下文对象的指针，只在当前上下文是由makecontext创建时有效
    struct ucontext_t *uc_link;
    // 当前上下文的信号屏蔽掩码
    sigset_t          uc_sigmask;
    // 当前上下文使用的栈内存空间，只在当前上下文是由makecontext创建时有效
    stack_t           uc_stack;
    // 平台相关的上下文具体内容，包含寄存器的值
    mcontext_t        uc_mcontext;
    ...
} ucontext_t;

// 获取当前的上下文
int getcontext(ucontext_t *ucp);

//恢复ucp指向的上下文，这个函数不会返回，而是会跳转到ucp上下文对应的函数中执行，相当于变相调用了函数
int setcontext(const ucontext_t *ucp);

//修改由getcontext获取到的上下文指针ucp，将其与一个函数func进行绑定，支持指定func运行时的参数，
//在调用makecontext之前，必须手动给ucp分配一段内存空间，存储在ucp->uc_stack中，这段内存空间将作为func函数运行时的栈空间，
//同时也可以指定ucp->uc_link，表示函数运行结束后恢复uc_link指向的上下文，
//如果不赋值uc_link，那func函数结束时必须调用setcontext或swapcontext以重新指定一个有效的上下文，否则程序就跑飞了
//makecontext执行完后，ucp就与函数func绑定了，调用setcontext或swapcontext激活ucp时，func就会被运行
void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...);

// 恢复ucp指向的上下文，同时将当前的上下文存储到oucp中，
//和setcontext一样，swapcontext也不会返回，而是会跳转到ucp上下文对应的函数中执行，相当于调用了函数
//swapcontext是sylar非对称协程实现的关键，线程主协程和子协程用这个接口进行上下文切换
int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);
*/

#ifndef __SYLAR_FIBER_H__
#define __SYLAR_FIBER_H__

#include <ucontext.h>

#include <functional>
#include <memory>

namespace sylar {

class Fiber : public std::enable_shared_from_this<Fiber> {
public:
    typedef std::shared_ptr<Fiber> ptr;

    /**
     * @brief: 协程状态
     */
    enum State {
        // 初始化
        INIT,
        // 暂停
        HOLD,
        // 执行中
        EXEC,
        // 结束
        TERM,
        // 可执行
        READY,
        // 异常
        EXCEPT
    };

private:
    /**
     * @brief: 无参构造
     * @attention: 私有，用于每个每个线程第一个线程的构造，即主协程
     */
    Fiber();

public:
    /**
     * @brief: 带参构造函数，用于构造子协程
     * @param[in] {std::function<void()>} cb  协程运行函数，协程入口
     * @param[in] {size_t} stacksize 协程栈大小
     * @param[in] {bool} use_caller 是否在MainFiber主协程上调度
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0,
          bool use_caller = false);

    /**
     * @brief: 析构函数
     */
    ~Fiber();

    /**
     * @brief: 重置协程执行函数，并设置状态
     * @param[in] {function<void()>} cb
     */
    void reset(std::function<void()> cb);

    /**
     * @brief: 将当前协程切换到运行状态
     */
    void swapIn();

    /**
     * @brief: 将当前协程切换到后台
     */
    void swapOut();

    /**
     * @brief: 返回协程id
     */
    uint64_t getId() const { return m_id; }

    /**
     * @brief: 返回协程状态
     */
    State getState() const { return m_state; }

public:
    /**
     * @brief: 设置当前线程的运行协程
     * @param[in] {Fiber*} f 运行协程
     */
    static void SetThis(Fiber* f);

    /**
     * @brief: 返回当前所在的协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief: 将当前协程切换到后台，并设置为READY状态
     */
    static void YieldToReady();

    /**
     * @brief: 将当前协程切换到后台，并设置为HOLD状态
     */
    static void YieldToHold();

    /**
     * @brief: 返回当前协程的总数量
     */
    static uint64_t TotalFibers();

    /**
     * @brief: 协程执行函数
     * @post: 执行完毕返回到线程主协程
     */
    static void MainFunc();

    /**
     * @brief: 获取当前协程的id
     */
    static uint64_t GetFiberId();

private:
    // 协程id
    uint64_t m_id = 0;
    // 协程运行栈大小
    u_int32_t m_stacksize = 0;
    // 线程状态
    State m_state = INIT;
    // 协程上下文
    ucontext_t m_ctx;
    // 协程运行栈指针
    void* m_stack = nullptr;
    // 协程运行函数，协程入口
    std::function<void()> m_cb;
};
}  // namespace sylar

#endif