#include <stdio.h>

#define dprint(expr) printf(#expr ": %d\n", (int)expr)
#define pprint(expr) printf(#expr ": %p\n", expr)
#define fprint(expr) printf(#expr ": %f\n", (float)expr)
#define sprint(expr) printf(#expr ": %s\n", (char *)expr)
#define cprint(expr) printf(#expr ": %c\n", (char)expr)
#define hprint(expr) printf(#expr ": 0x%x\n", expr)

