#include "Controller.h"
#include "Arduino.h"

void InitialState::handle(Context &context)
{
    //Serial.printf("Handling initial state\n");
    context.setState(std::make_unique<ConcreteStateA>());
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
    context.setState(std::make_unique<ConcreteStateB>());
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
    context.setState(std::make_unique<ConcreteStateA>());
}
