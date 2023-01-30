#include "lang.h"

#define fmt "Hi, %s. My name is %s!\n"

char *name = "Peter", *my_name = "Erika";

int salutation() {
    printf(fmt, name, my_name);
    return 5;
}