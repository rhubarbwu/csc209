# Commands

## Commands: An Assortment

`git`, `ssh`, `scp`, `echo`, `ls`, `cd`, `cat`, `grep`, `cut`, `sed`, `which`, `more`, `rm`, `man`, `sort`, ` find`, `who`, `wc`, `tr`, `last`, `tail`, `head`, `mv`, `cmp`, `diff`, `comm`, `join`, `awk`, `rev`, `exit`, `md5sum`, `for`, `while`, `if`, `read`, `case`, `watch`, `uniq`, `gcc`, `gdb`, `od`, `rev`, `free`, `file`, `ps`, `top`, `type/command`, `curl`, `wget`, `watch`, `python`, `ltrace`, `strace`, `valgrind`, `gdbgui`, `xargs`, `stat`, `vi`

Look these up in the man pages for practice!

## Commands: `man` pages $(\beta)$

![how it feels when you man](lec01/man_answer.jpeg){width=275}

## Commands: `man` pages $(\beta)$

- The man pages are sectioned; you will mainly use:
  - 1: General commands
    - e.g. `man ls` to learn how to use `ls`
  - 2: System calls
  - 3: Library functions
  - 7: Miscellanea
    - e.g. `man gittutorial` or `man man-pages` <!--for an introduction to git or man pages`, `respectively-->
- If the command exists in more than one section, specify the section you want:
  - `man 1 printf` for the `printf` shell command.
  - `man 3 printf` for the `printf` library function.

## Commands: `man` pages $(\beta)$

Even the `man` command has its own man page: `man man`

![a manual for man](lec01/man_man.jpeg){width=275}

You likely won't use any special options, aside from `man -k` or `man -K` (to search); `man man-pages` will be more generally informative.

## Commands: Compilers & Interpreters

- Shell is interpreted\*: `sh`, `bash`, `zsh`, etc.
- Python is usually interpreted\*: `python`
- Java is compiled to bytecode: `javac`
- C/C++ are compiled using `gcc`/`g++` or `clang`/`clang++`
- More about C later\...

\* interpreters typically written in C
