#include "Context.h"

void Context::setState(std::unique_ptr<State> newState) {
    if (state) {
        state->onExit(*this);
    }
    state = std::move(newState);
    if (state) {
        state->onEntry(*this);
    }
}

void Context::request() {
    if (state) {
        //TODO: collect sensor data and update the context private data

        state->handle(*this);
    }
}