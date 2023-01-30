#include "stdio.h"

int main() {
    char utm_local[] = "erindale",
         utm_city[] = "mississauga",
         utsg_local[] = "st. george",
         utsg_city[] = "toronto",
         utsc_local[] = "highland creek",  // try removing " creek"
        utsc_city[] = "toronto";

    utm_city[4] = '\0';  // city is now "miss"

    printf("%s, %s\n", utm_city, utm_local);
    utm_local[8] = 'i';  // what happens to utm_local?
    printf("%s, %s\n", utm_city, utm_local);
    utsg_local[10] = 'k';  // what about now?
    printf("%s, %s\n", utm_city, utm_local);
}