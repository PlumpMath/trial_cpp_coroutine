#pragma once

#include <csetjmp>
#include <cassert>

#include <functional>

template <typename T>
class CoroutineContext {
    std::jmp_buf outer_buf_;
    std::jmp_buf inner_buf_;
    bool has_inner_buf_ = false;
    bool is_continuable_ = true;

    using proc_t = std::function<void(CoroutineContext*)>;
    proc_t proc_;

    using return_t = T;
    return_t ret_;

public:
    CoroutineContext(proc_t proc) : proc_(proc) { assert(proc_); }

    bool is_continuable() const { return is_continuable_; }

    return_t operator()() {
        assert(is_continuable_);

        if (!setjmp(outer_buf_)) {
            if (has_inner_buf_) {
                has_inner_buf_ = false;
                std::longjmp(inner_buf_, 1);
            } else {
                volatile char stack[10000]={};
                (void)stack;
                proc_(this);
                is_continuable_ = false;
                ret_ = return_t();  // FIXME: 取り敢えずデフォルト値
                std::longjmp(outer_buf_, 1);
            }
        }

        return ret_;
    }

    void yield(const return_t& ret) {
        assert(!has_inner_buf_);

        has_inner_buf_ = true;
        if (!setjmp(inner_buf_)) {
            ret_ = ret;
            std::longjmp(outer_buf_, 1);
        }
    }
};
