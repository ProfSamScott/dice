#include <stdbool.h>

#define STATE_NUM 0
#define STATE_D 1
#define STATE_SIDES 2

typedef struct {int num; int sides;} diceset;

diceset makeSet(char *diceset_string); 
int rollset(diceset d, flags f); 