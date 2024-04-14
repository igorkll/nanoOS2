//yes, I'll just connect the shell in the dumbest way
//this approach will allow you to use all the system APIs from there without having to call include,
//since all include will already be called before loading customshell
#include "all.h"
#ifdef SYSTEM_CUSTOMSHELL
    #include "../profiles/_current/shell.c"
#endif