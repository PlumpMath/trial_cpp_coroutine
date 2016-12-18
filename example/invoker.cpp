
#include "invoker.h"

#include <QtDebug>

Invoker::Invoker(CoroutineContext<int>&& coro) : coro_(std::move(coro)) {
    timer_.setSingleShot(true);
    connect(&timer_, &QTimer::timeout, this, &Invoker::run);
}

void Invoker::run() {
    if (coro_.is_continuable()) {
        int msec = coro_();
        timer_.start(msec * 1000);
    } else {
        qDebug() << "Coroutine ended.";
    }
}
