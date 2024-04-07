#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Context.h"

void setupController(Context &context);

class InitialState : public State {
public:
    void onEntry(Context &context) override;
    void onExit(Context &context) override;
    void handle(Context& context) override;
};

class ConcreteStateA : public State {
public:
    void onEntry(Context &context) override;
    void onExit(Context &context) override;
    void handle(Context& context) override;
};

class ConcreteStateB : public State {
public:
    void onEntry(Context &context) override;
    void onExit(Context &context) override;
    void handle(Context& context) override;
};
#endif // CONTROLLER_H