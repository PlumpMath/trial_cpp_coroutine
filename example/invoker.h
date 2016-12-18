#pragma once

#include <QTimer>

#include "../coroutine.hpp"

class Invoker : public QObject {
    QTimer timer_;
    CoroutineContext<int>&& coro_;
public:
    Invoker(CoroutineContext<int>&& coro);

public slots:
    void run();
};
