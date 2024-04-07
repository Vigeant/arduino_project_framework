#include "Controller.h"
#include "Arduino.h"

static InitialState initialState;
static ConcreteStateA stateA;
static ConcreteStateB stateB;

void setupController(Context &context)
{
    context.setInitialState(&initialState);
}

void InitialState::handle(Context &context)
{
    //Serial.printf("Handling initial state\n");
    context.setState(&stateA);
}

void InitialState::onEntry(Context &context)
{
    //Serial.printf("Entering initial state\n");
}

void InitialState::onExit(Context &context)
{
    //Serial.printf("Exiting initial state\n");
}

void ConcreteStateA::onEntry(Context &context)
{
    //Serial.printf("Entering state A\n");
}

void ConcreteStateA::onExit(Context &context)
{
    //Serial.printf("Exiting state A\n");
}

void ConcreteStateA::handle(Context &context)
{
    //Serial.printf("Handling state A\n");
    context.setState(&stateB);
}

void ConcreteStateB::onEntry(Context &context)
{
    //Serial.printf("Entering state B\n");
}

void ConcreteStateB::onExit(Context &context)
{
    //Serial.printf("Exiting state B\n");
}

void ConcreteStateB::handle(Context &context)
{
    //Serial.printf("Handling state B\n");
    context.setState(&stateA);
}
