#include "lang.h"

int main() {  // perform a greeting
    int len = salutation();
    printf("%s: %d\n", name, len);
    return 0;
}