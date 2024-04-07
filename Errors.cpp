#include "Errors.h"

void printError(Error err)
{
    if (err == SUCCESS)
    {
        return;
    }
    Serial.print("Error: ");
    switch (err)
    {
    case SUCCESS:
        break;
    case TOO_MANY_ARGUMENTS:
        Serial.print("too many arguments");
        break;
    case TOO_FEW_ARGUMENTS:
        Serial.print("too few arguments");
        break;
    case VALUE_OUT_OF_RANGE:
        Serial.print("value is out of range");
        break;
    case SETTING_IS_READ_ONLY:
        Serial.print("setting is read only");
        break;
    case SETTING_NOT_FOUND:
        Serial.print("setting not found");
        break;
    case INVALID_ARGUMENT:
        Serial.print("invalid argument");
        break;
    }
}

void printErrorln(Error err){
    printError(err);
    Serial.print("\n");
}
void printErrorlnln(Error err){
    Serial.print("\n");
    printErrorln(err);
}