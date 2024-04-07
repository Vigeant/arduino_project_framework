# Arduino Project Framework

This project is meant to be a base skeleton for a configurable controller based on resource limmited arduino platforms. It provides the following functionalities:

## Settings
Settings are meant to be values that can be configured via a commandline without having to recompile and push the firmware on the platform. Sometime you just want to calibrate a sensor, or set some timer values and more.

## Command Line Interface
The project provides a command line interface that allows defining commands. The framework has all the commands to show, reset and set settings and serve as examples for creating new commands

## Finite State Machine
Via the Context and State Classes, it provides the building block to the finite state machine pattern and the Controller.cpp and .h provide an example on how to create new state and set the transitions.

## Common Errors
Via the Error.h file, it provides the funtionality to create and print new error messages that are specific to this project

## TODO
Logging that support loglevels from the settings.
