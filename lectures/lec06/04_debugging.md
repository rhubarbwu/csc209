# Debugging Tools: `gdb` and `valgrind`

## Debugging Tools for C

Thanks to the widespread use of C/C++, there are lots of tools to help you analyse, debug, and optimize your code.

### Static Analysis

Static code analyzers parse through source code and libraries to ensure that types, imports, and pointers make sense before you even execute the code.

- They include the compilers you're already familiar with (`gcc`/`g++`/`clang`/`clang++`).

### Dynamic Analysis

- Common tools for dynamic analysis (on variables/objects during execution) include debuggers like `gdb` and `lldb` ([`lldb.llvm.org/`](https://lldb.llvm.org/)).
- Some integrated development environments (IDEs) like Visual Studio (Code) and JetBrains CLion and have their own or wrappers around existing debuggers.
- Another useful tool for analysing memory structures and integrity is `valgrind`.

## Why Use a Debugger?

![admit it, we all do this](figures/debugging-drake.png){width=72%}

## Why Use a Debugger?

![debugging can be time-consuming](figures/debugging-everyday.png){width=60%}

## Why Use a Debugger?

![sometimes you actually want to understand](figures/debugging-why.png){width=53%}

## GDB: The GNU Project Debugger

The GNU Project debugger (GDB) allows you to see what is going on `inside' another program while it executes -- or what another program was doing at the moment it crashed.

GDB can do four main kinds of things (plus other things in support of these) to help you catch bugs in the act:

- Start your program, specifying anything that might affect its behavior.
- Make your program stop on specified conditions.
- Examine what has happened, when your program has stopped.
- Change things in your program, so you can experiment with correcting the effects of one bug and go on to learn about another.

Programs might be executing on the same machine as GDB (native), on another machine (remote), or on a simulator. GDB can run on most popular UNIX and Windows variants, as well as on Mac OS X. In addition to C/C++, GDB supports:

- Assembly, Ada, D, Fortran, Go, Objective-C, OpenCL, Modula-2, Pascal, Rust

Source: [`sourceware.org/gdb/`](https://sourceware.org/gdb/)

## GDB: How to Get Started

Firstly, include the `-g` flag in `gcc/clang`. And then prepend `gdb` to the command.

```sh
gcc -g -o main main.c

gdb ./main # run a simple command
gdb --args ./main arg1 arg2 ... # run with arguments to main
```

### Breakpoints & Run

At certain points in the program you want to see the value of certain variables. You can use `break` (`b`) to set suchs breakpoints at non-blank/comment lines.

```sh
(gdb) break 5
Breakpoint 1 at 0x126e: file main, line 5.
(gdb) break 128
Breakpoint 2 at 0x1304: file main, line 129.
```

And finally, use `run` to start the debugger.

```sh
(gdb) run
Starting program: ./main arg1 arg2
```

## GDB: Stepping Through the Program

Once you start, you have several commands for stepping. Here are some basics.

| commands       | what it does                                               |
| -------------- | ---------------------------------------------------------- |
| backtrace (bt) | Print backtrace of all stack frames, or innermost frames.  |
| break (b)      | Set breakpoint at specified (line) location.               |
| dump           | Dump target code/data to a local file.                     |
| exit/quit (q)  | Exit the debugger.                                         |
| next (n)       | Step program, proceeding through subroutine calls.         |
| nexti (ni)     | Step one instruction, but proceed through subroutine calls |
| print (p)      | Print value of a variable or expression.                   |
| step (s)       | Step program until it reaches a different source line.     |
| stepi (si)     | Step one (lower-level) instruction exactly.                |

You can use `help` to see more instructions and `help all` for a list of commands.

## GDB: Additional Info

- Enabling `gdb` with `-g` makes the binary bigger. Feel free to leave it out if you're trying to produce the slimmest possible application that you feel has been debugged enough.
- `gdb` itself is a command-based application, but there exist front-ends and wrappers ranging in complexity from `ddd` and `cgdb` all the way to Visual Studio.

### Back to `minigrep`

Try debugging `minigrep.c` to fix it!

```sh
# gdb is enabled in some other compilers too!
clang -g -o minigrep minigrep.c
# debug in searching for while loops or macros
gdb --args minigrep minigrep.c while
gdb --args minigrep minigrep.c \#define
```

- What breakpoints might be interesting?
- Which variables/expressions?

Full code: [`mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/minigrep.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/minigrep.c)

## GDB: Additional Info

- Enabling `gdb` with `-g` makes the binary bigger. Feel free to leave it out if you're trying to produce the slimmest possible application that you feel has been debugged enough.
- `gdb` itself is a command-based application, but there exist front-ends and wrappers ranging in complexity from `ddd` and `cgdb` all the way to Visual Studio.

### Back to `minigrep`

Try debugging `minigrep.c` to fix it!

```sh
# gdb is enabled in some other compilers too!
clang -g -o minigrep minigrep.c
# debug in searching for while loops or macros
gdb --args minigrep minigrep.c while
gdb --args minigrep minigrep.c \#define
```

- What breakpoints might be interesting? `break 18`, `break 29`.
- Which variables/expressions? At `29`: `line`, `line+i`, `pattern` and/or `result`.

Full code: [`mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/minigrep.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/minigrep.c)

## Valgrind: What is it?

![Depiction of Saint George and the dragon.](figures/st-george-dragon.png){width=60%}

Valgrind is an instrumentation framework for building dynamic analysis tools. There are Valgrind tools that can automatically detect many memory management and threading bugs, and profile your programs in detail. You can also use Valgrind to build new tools.

Source: [`valgrind.org/`](https://valgrind.org/)

## Valgrind: What Kinds of Build Tools?

![Relevance? Look up the green/red/purple "dragon book".](figures/st-george-dragon.png){width=38%}

The Valgrind distribution currently includes seven production-quality tools:

- a memory error detector (Memcheck, the _default_)
- two thread error detectors
- a cache and branch-prediction profiler
- a call-graph generating cache and branch-prediction profiler
- two different heap profilers

It also includes an experimental SimPoint basic block vector generator.

Source: [`valgrind.org/`](https://valgrind.org/)

## Valgrind: In Action (Memory Permissions)

```c
#include <stdlib.h>
void f(void) {
    int* x = malloc(10 * sizeof(int));
    x[10] = 0;  // problem 1: heap block overrun
}
int main(void) {
    f();
    return 0; }
```

Running `valgrind ./example` might show you something like this.

```python
==19182== Invalid write of size 4
==19182==    at 0x804838F: f (example.c:4)
==19182==    by 0x80483AB: main (example.c:7)
==19182==  Address 0x1BA45050 is 0 bytes after a block of size 40 alloc'd
==19182==    at 0x1B8FF5CD: malloc (vg_replace_malloc.c:130)
==19182==    by 0x8048385: f (example.c:3)
==19182==    by 0x80483AB: main (example.c:7)
```

## Valgrind: In Action (Memory Permissions)

What does this stuff mean?

```python
==19182== Invalid write of size 4
==19182==    at 0x804838F: f (example.c:4)
==19182==    by 0x80483AB: main (example.c:7)
==19182==  Address 0x1BA45050 is 0 bytes after a block of size 40 alloc'd
==19182==    at 0x1B8FF5CD: malloc (vg_replace_malloc.c:130)
==19182==    by 0x8048385: f (example.c:3)
==19182==    by 0x80483AB: main (example.c:7)
```

- The process ID (PID) is 19182.
- The first line indicates an "Invalid write": you wrote out of bounds on the heap.
- Below the first line is the stack trace; read it bottom-up.
- The code addresses (eg. 0x804838F) are _usually_ unimportant.
- Some error messages have a second component describing the memory address involved.
  - This one shows that the written memory is just past the end of a block allocated with malloc() on line 5 of example.c.

## Valgrind: In Action (Memory Leaks)

```c
#include <stdlib.h>
void f(void) {
    int* x = malloc(10 * sizeof(int));
    x[10] = 0;  // problem 1: heap block overrun
}               // problem 2: memory leak -- x not freed
```

You can include `--leak-check=[yes|full]` for detailed info on memory leaks. Running `valgrind --leak-check=yes ./main` might show you something like this.

```python
==19182== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==19182==    at 0x1B8FF5CD: malloc (vg_replace_malloc.c:130)
==19182==    by 0x8048385: f (example.c:4)
==19182==    by 0x80483AB: main (example.c:7)
```

- "definitely lost": your program is leaking memory -- fix it!
- "probably lost": your program is leaking memory, unless you have some way of backtracking on the heap.

Source: [`valgrind.org/docs/manual/quick-start.html`](https://valgrind.org/docs/manual/quick-start.html)

## Debugging: Correctness vs. Optimization

Because using `-g` to enable `gdb` (and others) adds debugging information to the application in the end, the binary will be larger and less efficient.

```sh
gcc -o example example.c
gcc -g -o example-g example.c
wc -l example example-g && du -h example example-g
```

- You can use `wc -l` or `du -h` to count number of lines or bytes a binary takes up.

Debugging is for the purpose of correctness, which is the most important thing.

- Once you're satisfied that your code is correct (as can be), remove `-g` to compile more compact and efficient code.

### Optimization: [`gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html`](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)

Conversely, the more optimization you include (`-O1`, `-O2`, `-O3`, etc.), the less helpful or reliable valgrind and other tools can become (especially for complex programs).

### Example: Leaky Linked-List

Full code: [`mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/leakyll.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/rupert/w06/leakyll.c)
