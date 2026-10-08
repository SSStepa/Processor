#include "comp.h"

int main()
{
    SetUpLog();

    char command[20] = "";
    double arg = 0;
    while (1) {
        GetCommand(command, &arg);
        printf("Hi");
        $str(command);
        $dbl(arg);
    }
    
    return 0;
}