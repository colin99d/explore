#include "main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4
#define MAX_LEN 10

void discovery_menu(Player *user,int width, char *string, char* selection);
void result_menu(Player *user, int width, int location);
