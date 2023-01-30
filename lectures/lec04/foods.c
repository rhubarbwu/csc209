#include "stdio.h"
#include "strings.h"

// whether or not the null-terminator counts
// #define len strlen
#define len len_incl_null

int len_incl_null(char *some_str)
{
    int l = 0;
    for (char *ptr = some_str;; ptr++)
    {
        l++;
        if (*ptr == '\0')
            return l;
    }
    return -1;
}

int main()
{
    char fruit[6] = {'a', 'p', 'p', 'l', 'e'}, // fruit[5] is \0
        party[6] = "nachos",                   // no space for \0
        movie[6] = "popcorn";                  // n is truncated, no \0
    printf("%s %d\n", fruit, len(fruit));
    printf("%s %d\n", party, len(party));
    printf("%s %d\n", movie, len(movie));

    char bread[13] = "sourdough"; // try changing this to 5 or 13
    printf("%s %d\n", fruit, len(fruit));
    printf("%s %d\n", party, len(party));
    printf("%s %d\n", movie, len(movie));
    printf("%s %d\n", bread, len(bread));
    return 0;
}