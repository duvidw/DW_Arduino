#pragma once

#include <Arduino.h>
#include <String.h>

class DWString : public String
{
    public : 
    DWString();
    int GetCommandAndValue(String &strVal);
};
