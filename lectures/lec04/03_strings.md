# Strings: Just Spicy Arrays

## Strings: Just Spicy Arrays

"Strings" in C are actually a special case of `char` arrays: they're **null-terminated**, meaning the last _actual_ character is `\0`.

```c
char limited[9]; // such a string shouldn't exceed 8
```

- When working with strings, `\0` isn't typically used/printed.
- Instead, it indicates where the string ends.

  - If you're writing a function that doesn't know the exact length of the string, the `\0` might come in handy.
  - Inserting a `\0` in the middle of a `char *` shortens the effective string.
    ```c
    char *city = "mississauga";
    city[4] = '\0'; // city is now "miss"
    ```

- Declaring strings with explicit length initializes remainder as `\0`.
- Important for many string-wise functions.

## Strings: Delicate Arrays

String manipulation in practice (if not carefully done) often results in unexpected/inconsistent behaviour or memory/pointer errors.

```c
int main() {
    char utm_local[] = "erindale"
         utm_city[] = "mississauga",
         utsg_local[] = "st. george",
         utsg_city[] = "toronto";
    utm_city[4] = '\0';  // utm_city is now "miss"

    utm_local[8] = 'i'; // what happens to utm_local?
    utsg_local[10] = 'k'; // what about now?
}
```

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec04/campuses.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec04/campuses.c)

## Strings: Delicate Arrays

Depending on stack memory layout and changes, removing `\0` might lead to something "harmless" like inconsistent reading overruns.

```c
#include "strings.h" // using strlen
int main() {
    char F[6] = "{'a', 'p', 'p', 'l', 'e'}", // F[5] = \0
        P[6] = "nachos",  // no space for \0
        M[6] = "popcorn"; // n is truncated, no \0
    printf("%d %d %d\n", len(F), len(P), len(M));

    char B[9] = "sourdough";  // try len = 5 or 13
    printf("%d %d %d\n", len(F), len(P), len(M));
    return 0;
}
```

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec04/foods.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec04/foods.c)
