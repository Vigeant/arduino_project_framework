#ifndef CONTEXT_H
#define CONTEXT_H

#include <memory>

class Context;

class State {
public:
    virtual ~State() {}

    // Called when entering a state
    virtual void onEntry(Context& context){}

    // Called when exiting a state
    virtual void onExit(Context& context){}

    virtual void handle(Context& context) = 0;
};

class Context {
private:
    std::unique_ptr<State> state;

public:
    Context(std::unique_ptr<State> state) : state(std::move(state)) {}
    void setState(std::unique_ptr<State> newState);
    void request();
    State* getState() const { return state.get(); }  // Added getter for the state
};

#endif // CONTEXT_H