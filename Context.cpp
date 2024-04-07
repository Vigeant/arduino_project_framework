#include "Context.h"


void Context::setState(State* newState) {
    if (state) {
        state->onExit(*this);
    }
    state = newState;
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