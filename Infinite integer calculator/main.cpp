#include "StringStack.h"
#include "Check.h"
#include <cstring>
#include <stdlib.h>
#include <string>
#include <algorithm>


int main()
{
	string str = "a*a$";
	Check a = Check(str);
    while (1) {
        string str;
        cout << "Input : ";
        getline(cin, str);
        str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
        if (str.length() == 0)
        {
            cout << "Error : Blank Input" << endl;
            continue;
        }
        if (str == "0")
        {   
            cout << endl << "Exit Program..." << endl;
            break;
        }
        cout << "Output: " << StringStack(str).calculate() << endl;
    }
    return 0;
}