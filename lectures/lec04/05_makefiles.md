# Build Automation (Makefiles)

## Automation with Scripts

Now, suppose we are writing a language translation program that uses an intermediate representation (IR) of type `int *` of length $2048$.

- Input from the source language is _encoded_ to the IR.
- Output to the target language is _decoded_ from the IR.
- Don't worry about the implementation of `encode()` and `decode()`.
- Suppose for every language `xx`, we have `xx-e.c`...
  ```c
  int *encode(char *input);
  ```
  And `xx-d.c`...
  ```c
  char *decode(int *ir);
  ```
- And that `main()` could call either of these at will.

How would you design and build this?

## Automation with Scripts

One solution is to put `encode()` and `decode()` in a common header(s) and compile each language pair manually.

```sh
gcc -o ar-bn ar-e.c bn-d.c main.c
gcc -o ar-de ar-e.c de-d.c main.c
gcc -o ar-en ar-e.c en-d.c main.c
gcc -o ar-es ar-e.c es-d.c main.c
gcc -o ar-fr ar-e.c fr-d.c main.c
gcc -o ar-hi ar-e.c hi-d.c main.c
gcc -o ar-jp ar-e.c jp-d.c main.c
gcc -o ar-pt ar-e.c pt-d.c main.c
gcc -o ar-ru ar-e.c ru-d.c main.c
gcc -o ar-zh ar-e.c zh-d.c main.c
...
```

We might want to write a script/function to generalize...

```sh
gcc -o $1-$2 $1-e.c $2-d.c.
```

## Automation with Scripts

You could even batch it...

```sh
#!/bin/sh
langs="ar bn de en es fr hi jp pt ru zh"
for l1 in $langs; do
    for l2 in $langs; do
        gcc -o $1-$2 $l1-e.c $l2-e.c
    done
done
```

This is much better, right? More elegant and programmable for sure.

## Automation with Scripts

You could even batch it...

```sh
#!/bin/sh
langs="ar bn de en es fr hi jp pt ru zh"
for l1 in $langs; do
    for l2 in $langs; do
        gcc -o $1-$2 $l1-e.c $l2-e.c
    done
done
```

Is this efficient? Definitely not! Every time you need some encoder `xx-e` or decoder `yy-d`, you're recompiling the same object but not reusing it.

## Makefiles: Introduction

- Makefiles facilitate building (i.e., compiling, linking, sometimes testing and packaging) projects consisting of multiple source files.
- If only one source file has changed, no need to recompile everything; instead:
  1. Recompile source files that have changed.
  2. Relink updated object files to generate new executable file.

### Makefiles: Format

A Makefile contains a sequence of rules, each in the format:

```makefile
target: prereq_1 prereq_2 ... prereq_n
    action_1
    ...
    action_n
```

## Makefiles: Usage

Makefiles are processed by the make program

- Run `make` with no arguments to evaluate first rule.
- Run `make TARGET` to execute action(s) defined in rule for
  `TARGET`.
  - Only if `TARGET` prerequisites were modified since last time that
    `make TARGET` was run.
- To force `make TARGET` to recompile code, you can:
  - Update last modified time of prerequisite source files, with `touch`; or
  - Delete prerequisite object files.

## Makefile Syntax: Defining Variables

You may define variables; e.g., to store compiler flags:

```makefile
CFLAGS= -g -Wall -Werror -fsanitize=address

reverse : reverse.c
    gcc $(CFLAGS) -o reverse reverse.c
```

You can even declare an alternative compiler.

```makefile
CXX=clang++

forward : forward.c
    gcc $(CFLAGS) -o forward forward.c
```

## Makefile Syntax: Automatic (Built-In) Variables

| Variable | Meaning                       |
| -------- | ----------------------------- |
| `$@`     | Target                        |
| `$<`     | First prerequisite            |
| `$?`     | All out of date prerequisites |
| `$^`     | All prerequisites             |

```makefile
CFLAGS= -g -Wall -Werror -fsanitize=address

hello: hello.c hello.h
    gcc $(CFLAGS) -o $@ $<
```

Ref.: [10.5.3: Automatic Variables, GNU Make manual](https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html)

## Makefile Example: Past Assignment

```makefile
FLAGS= -Wall -Werror -fsanitize=address -g
OBJ = simfs.o initfs.o printfs.o simfs_ops.o
DEPENDENCIES = simfs.h simfstypes.h

all : simfs

simfs : ${OBJ}
    gcc ${FLAGS} -o $@ $^

%.o : %.c ${DEPENDENCIES}
    gcc ${FLAGS} -c $<

clean :
    rm -f *.o simfs
```

## Makefile Example: Pattern Rules

```makefile
%.o : %.c ${DEPENDENCIES}
    gcc ${FLAGS} -c $<
```

- Most files are compiled in the same way, so we write
  a pattern rule for the general case
- `%` expands to the stem of the file name (i.e., without extension)
- `gcc -c` compiles the source file(s), but does not link

## Makefile Example: Phony Targets

You may want a command that builds a target:

```makefile
OBJ = simfs.o initfs.o printfs.o simfs_ops.o

simfs: ${OBJ}
    gcc ${FLAGS} -o $@ $^
```

Or a target that doesn't build anything:

```makefile

clean:
    rm -f *.o simfs
```

# Makefiles: Practice

Provided by Karen Reid from my CSC209H1 in Fall 2019, supposedly an assignment from an even older offering of CSC209.

## Makefiles: Practice (1)

```makefile
test_print: test_print.o ptree.o
    gcc -Wall -g -std=gnu99 -o test_print test_print.o ptree.o
```

1. What's the target?
2. What're the prerequisites? What's another term for them?
3. How many actions does this rule have?
4. What does a file that ends in `.o` contain? How is it generated?

## Makefiles: Practice (2)

```makefile
FLAGS = -Wall -g -std=gnu99
DEPENDENCIES = ptree.h
all: test_print print_ptree
test_print: test_print.o ptree.o
    gcc ${FLAGS} -o $@ $^
print_ptree: print_ptree.o ptree.o
    gcc ${FLAGS} -o $@ $^
%.o: %.c ${DEPENDENCIES}
    gcc ${FLAGS} -c $<
clean:
    rm -f *.o test_print print_ptree
```

1. If we were to run `make print_ptree` which rule is evaluated first?
2. What new files would be created?
3. What is the last action that is executed in the make command above?

## Makefiles: Practice (3)

```makefile
FLAGS = -Wall -g -std=gnu99
DEPENDENCIES = ptree.h
all: test_print print_ptree
test_print: test_print.o ptree.o
    gcc ${FLAGS} -o $@ $^
print_ptree: print_ptree.o ptree.o
    gcc ${FLAGS} -o $@ $^
%.o: %.c ${DEPENDENCIES}
    gcc ${FLAGS} -c $<
clean:
    rm -f *.o test_print print_ptree
```

4. Which files will the pattern rule `(%.o : %.c)` match on?
5. If we the modify `ptree.c` and run `make print_ptree` again, which rules are evaluated? Which actions are executed?
