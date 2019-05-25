#include "DWString.h"

DWString::DWString()
{
}

int DWString::GetCommandAndValue(String &strVal)
{
    String strInp = *this;
    String strCmd;
    int iCommand = 0;
    Serial.print(">>>");
    Serial.print(strInp);
    Serial.println("<<<");

    // if '=' exists, return the int and strVal fill with the string after the =
    int i1 = strInp.indexOf('=');
    if (i1 < 0)
    {
        iCommand = strInp.toInt();
    }
    else if (i1 == 0)
    {
        strVal = strInp.substring(i1 + 1);
    }
    else
    {
        strCmd = strInp.substring(0, i1);
        strVal = strInp.substring(i1 + 1);
        iCommand = strCmd.toInt();
    }
    return iCommand;
}
