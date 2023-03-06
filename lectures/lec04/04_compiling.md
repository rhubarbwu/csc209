# Compilation

## Compilation: C Programs

- C programs can consist of multiple `*.c` files
- Each individual `*.c` file can be compiled to an object file.
- Object files (`*.o`) contain "placeholders" for addresses of functions that were declared but not defined.
  - Header (`*.h`) files ensure consistency between function
    declarations across your program’s multiple source files.
- The linker connects object files together to create an
  executable file.

## Compilation: GCC Pipeline ($\rho$)

Recall that all input files (the last arguments) flow through the pipeline (depending on options) up to (and including)...

1. Preprocessing (`gcc -E`) strips comments and expands directives.
2. Compilation (`gcc -S`) generates assembly code (`*.s`/`*.asm`).
3. Assembly (`gcc -c`) generates binary/machine code objects (`*.o`).
4. Linking (`gcc`) consolidates objects into a single application.

   - defaults to `a.out`, but you can use `-o <output>` to specify.

## Compilation: Preprocessing

Preprocessing...

- removes comments.
- expands compiler directives
  - `#includes` statements (akin to imports).
  - `#define` macros.

You can emit preprocessed code with `gcc -E`.

```sh
$ gcc -E foods.c
$ gcc -dM -E foods.c # includes pre-defined macros
```

## Compilation: Separate Compilation

Seems like a lot of steps, right? It's like building a large project bottom-up with the ability to mix/match components when necessary/desired. An analogy could be assembling automobiles.

| Automobile Manufacturing                | Compiling with GCC             |
| --------------------------------------- | ------------------------------ |
| Extract raw materials                   | Preprocess compiler directives |
| Produce basic parts                     | Compile to assembly            |
| Assemble larger parts like the engine   | Assemble to binary objects     |
| Assemble together, connect pipes/wires, | Link to an application         |
| screw/plug everything else, etc.        |                                |

At any point, different parts can be chosen/substituted. The interface just has to be valid (recall CSC207) as per the header files (`*.h`).

## Compilation: Separate Compilation

Suppose you want the following English code... (`en.c`)

```c
#include <stdio.h>
#define fmt "Hi, %s. My name is %s too!\n"
char *name = "Peter", *my_name = "Erika";
int salutation() {
    printf(fmt, name, my_name);
    return 5;
}
int main() {
    printf("%s: %d\n", name, salutation());
    return 0;
}
```

Notice the `#include` imports and macro `fmt`.

## Compilation: Separate Compilation

But you want to make a French version too... (`fr.c`)

```c
#include <stdio.h>
#include <string.h>
char* name = "Pierre";
int salutation() {
    printf("Bonjour, %s.\n", name);
    return strlen(name);
}
int main() {
    printf("%s: %d\n", name, salutation());
    return 0;
}
```

You could write separate programs, but what if they're largely similar? Can we flexibly reuse consistent code?

## Compilation: Separate Compilation

Yes! Tie them together with a common header file (such as `lang.h`).

```c
#include <stdio.h>
#include <string.h>
extern char* name; // global declaration
void salutation();
```

And use it as an interface by `#include` directive.

```c
#include "lang.h"
int main() {  // perform a greeting
    salutation();
    printf("%s: %d\n", name, strlen(name));
    return 0;
}
```

Notice `salutation()` now returns `void` instead of `int`. Why? How?

## Compilation: Separate Compilation

Here's what French (`fr.c`) might look like.

```c
#include "lang.h"
char* name = "Pierre";
void salutation() {
    printf("Bonjour, %s.\n", name);
}
```

And English (`en.c`) ...

```c
#include "lang.h"
#define fmt "Hi, %s. My name is %s too!\n"
char *name = "Peter", *my_name = "Erika";
void salutation() {
    printf(fmt, name, my_name);
}
```

## Compilation: Separate Compilation

Finally, compile for the language you want. Here the objects are compiled separately for clarity and reuse; you can link complete binaries without recompiling the components each time.

```sh
gcc -S en.c             # compile en.s
gcc -c en.s             # assemble en.o
gcc -o en en.o greet.c  # link English binary
./en                    # run English binary

gcc -c en.c fr.c        # create en.o and fr.o
gcc -o fr fr.o greet.c  # link French binary
./fr                    # run French binary
```

### Compilation: Arnold's Examples

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/c/logistics.zip`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/c/logistics.zip)
