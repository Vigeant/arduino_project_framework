#ifndef ERRORS_H
#define ERRORS_H

#include "Arduino.h"

enum Error
{
    SUCCESS = 0,
    TOO_MANY_ARGUMENTS,
    TOO_FEW_ARGUMENTS,
    VALUE_OUT_OF_RANGE,
    SETTING_IS_READ_ONLY,
    SETTING_NOT_FOUND,
    INVALID_ARGUMENT,
};

void printError(Error err);
void printErrorln(Error err);
void printErrorlnln(Error err);

#endif /* ERRORS_H */