#include <string.h>

#include "lang.h"

char* name = "Pierre";

int salutation() {
    printf("Bonjour, %s.\n", name);
    return strlen(name);
}