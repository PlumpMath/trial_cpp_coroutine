
#include "../coroutine.hpp"
#include "invoker.h"

#include <QtDebug>
#include <QApplication>

class Command {
public:
    virtual ~Command(){}
    virtual void exec(CoroutineContext<int>* coro) = 0;
};

class CmdList : public Command {
    QList<Command*> list_;

public:
    void exec(CoroutineContext<int>* coro) override {
        for (auto&& c : list_) {
            c->exec(coro);
        }
    }

    ~CmdList() {
        for (auto&& c : list_) {
            delete c;
        }
    }

    void push_back(Command* cmd) { list_.push_back(cmd); }
};

class CmdPrint : public Command {
    const QString msg_;

public:
    CmdPrint(QString msg) : msg_(msg) {}
    void exec(CoroutineContext<int>*) override {
        qDebug() << Q_FUNC_INFO << msg_;
    }
};

class CmdSleep : public Command {
    const int sec_;

public:
    CmdSleep(int sec) : sec_(sec) {}

    void exec(CoroutineContext<int>* coro) override { coro->yield(sec_); }
};

int
main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    auto cmdlist = new CmdList;
    cmdlist->push_back(new CmdPrint("CmdPrint1"));
    cmdlist->push_back(new CmdSleep(1));
    cmdlist->push_back(new CmdPrint("CmdPrint2"));
    cmdlist->push_back(new CmdSleep(1));
    cmdlist->push_back(new CmdPrint("CmdPrint3"));
    cmdlist->push_back(new CmdSleep(1));

    CoroutineContext<int> ctx([cmdlist] (CoroutineContext<int>* coro) {
            cmdlist->exec(coro);
        });

    Invoker invoker(std::move(ctx));
    invoker.run();

    return app.exec();
}
