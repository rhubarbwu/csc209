# Strings Manipulation

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

## Strings Manipulation: Some Library Functions (`string.h`)

Many string manipulation functions can be found in `string.h`.

```c
// copies the string pointed to, by src to dest.
char *strcpy(char *dest, const char *src);

// appends the string pointed to, by src to the
// end of the string pointed to by dest.
char *strcat(char *dest, const char *src);

// computes the length of the string str up to
// but not including the terminating null character.
size_t strlen(const char *str);

// compares the string pointed to,
// by str1 to the string pointed to by str2.
int strcmp(const char *str1, const char *str2);
```

## Strings Manipulation: More Library Functions (`stdio.h`/`string.h`)

`sprintf` and `gets` are provided by `stdio.h`.

```c
// sends formatted output to a string.
int sprintf(char *str, const char *format, ...)

// get string from stdin.
// discontinued in C11/C++14. Why?
char *gets (char *str);
```

### C23: The next generation of the C standard

In the C23 standard, `strdup` was introduced. It returns a pointer to a null-terminated byte string on the heap, which is a duplicate of the string pointed to by `str1`.

```c
char *strdup(const char *str1);
```

## Strings Manipulation: Memory Errors

![you hate to see it](lec06/printf-string.jpeg){width=95%}

## Strings Manipulation: Manipulation & Memory Safety

- String manipulation is a major cause of memory errors (e.g., _buffer overflow_)
- The C standard library includes both _safe_ and _unsafe_ string functions

  - **Some** unsafe functions can be used safely if the string is **guaranteed** to be NULL-terminated:

    ```c
    // unsafe, only safe if argv is guaranteed NULL-terminated
    strlen(argv[0]);
    ```

  - But even so-called "safe" functions can cause memory errors if used improperly:

    ```c
    // safe, but vulnerable to improper use
    char x[2]; strncpy(x, "blabla", 7);
    ```

## Strings Manipulation: Safety Manual

From `man gets` (Linux):

> Never use gets(). Because it is impossible to tell without knowing the
> data in advance how many characters gets() will read, and because
> gets() will continue to store characters past the end of the buffer, it
> is extremely dangerous to use. It has been used to break computer
> security. Use fgets() instead.

Also from `man gets` (Mac):

> The gets() function cannot be used securely. Because of its lack of bounds checking, and the inability for the calling program to
> reliably determine the length of the next incoming line, the use of this function enables malicious users to arbitrarily change a running program's functionality through a buffer overflow attack. It is strongly suggested that the fgets() function be used in all cases.

## Strings Manipulation: Unsafe Functions & Security

- _Question_: How can an attacker exploit a buffer overflow to break a system's security?
  - _Answer_: One way: [`samsclass.info/127/proj/p3-lbuf1.htm`](https://samsclass.info/127/proj/p3-lbuf1.htm)
- _Real example_: WhatsApp vulnerability from May 2019.
  - [`arstechnica.com/information-technology/2019/05/whatsapp-vulnerability-exploited-to-infect-phones-with-israeli-spyware/`](https://arstechnica.com/information-technology/2019/05/whatsapp-vulnerability-exploited-to-infect-phones-with-israeli-spyware/)

### Lessons

- Only use C when necessary, and be mindful of safe programming practices.
- Otherwise, be responsible and use the right language for your task.

## Strings Manipulation: What's Safe? What isn't?

<!-- Figure with Drake, unsafe string functions on top half (strcpy, strcat, strlen, sprintf, gets: Drake disapproves) and same versions with 'n' below strncpy, strncat, strnlen, snprintf, fgets (Drake approves) -->

![What Drake thinks about `strings.h`/`stdio.h`.](lec06/safe-functions.png){width=75%}

## Strings Manipulation: Safe(r) Library Functions

Just like with normal arrays, it's better practice to use functions that restrict or explicitly state the maximum number of characters.

```c
// from string.h
char *strncpy(char *dest, const char *src, size_t num);
char *strncat(char *dest, const char *src, size_t num);
size_t strnlen(const char *s, size_t num);
char *strndup( const char *str, size_t size ); // C23

// from stdio.h
int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...);
char *fgets(char *restrict s, int n, FILE *restrict stream);
```

### Safety is Always Evolving

Newer versions and languages sometimes (re-)implement safer functions. For example, C11 Annex K introduces `strnlen_s`, whose distinction from `strnlen` is that it checks if `const char *s` is `NULL` and returns `0` if so; what would `strnlen` do?

## Strings Manipulation: Exercises in Safety (1)

![How about these?](lec06/strcpy-meme.png){width=70%}

## Strings Manipulation: Exercises in Safety (1)

**Is this safe?**

```c
char *dest;
strcpy(dest, src);
```

**What about these?**

```c
#define MAXS 100
char dest[MAXS];

strncpy(dest, src, MAXS);

strncpy(dest, src, MAXS - 1);

dest[0] = '\0';
strncat(dest, src, MAXS - 1);
```

## Strings Manipulation: Exercises in Safety (1)

**Is this safe?** _No._

```c
char *dest;
strcpy(dest, src); // unsafe
```

**What about these?** _No, yes, and yes._

```c
#define MAXS 100
char *dest = malloc(sizeof(char) * MAXS); // same issues, stack or heap

strncpy(dest, src, MAXS); // unsafe: maybe no null-terminator

strncpy(dest, src, MAXS - 1); // safe

dest[0] = '\0'; // it gets overwritten
strncat(dest, src, MAXS - 1); // safe, same as above
```

## Strings Manipulation: Exercises in Safety (2)

**How about these?**

```c
char str1[20] = "BeginnersBook";
printf("Length of string str1 %d\n", strnlen(str1, 30));
printf("Length of string str1 %d\n", strnlen(str1, 10));

char str2[6] = "csc209";
// some other code here.
int b;
scanf("%d", &b);
printf("Length of string str2 %d\n", strnlen(str2, b));

char buf[209] = {'\0'};
printf("Enter your name and press <Enter>\n");
gets(buf);
```

## Strings Manipulation: Exercises in Safety (2)

**How about these?** _Yes, no, and no._

```c
char str1[20] = "BeginnersBook";
printf("Length of string str1 %d\n", strnlen(str1, 30)); // safe
printf("Length of string str1 %d\n", strnlen(str1, 10)); // safe

char str2[6] = "csc209";
// some other code here. might've removed the \0 from str2!
int b;
scanf("%d", &b); // unsafe: never trust external input
printf("Length of string str2 %d\n", strnlen(str2, b));

char buf[209] = {'\0'};
printf("Enter your name and press <Enter>\n");
gets(buf); // unsafe: accepts infinite stream
```

## Strings Manipulation: Example (`minigrep.c`)

Let's implement a miniature version of `grep` called `minigrep`. It'll simply search for lines matching `pattern` and print out match positions. More precisely, for each `line` in your `file`, our implementation will see if there exists some `size_t i` such that `strcmp(line+i, pattern)`. No flags, and exactly a `filename` and `pattern` are provided as arguments. Compile and run it on its own source code:

```sh
$ gcc -o minigrep minigrep.c
$ minigrep minigrep.c \#define
```

You'll find that the given code doesn't actually work! Replace `strcmp` with `strncmp` if the `line` is expected to be longer than `pattern`.

### Full code

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec06/minigrep.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec06/minigrep.c)
