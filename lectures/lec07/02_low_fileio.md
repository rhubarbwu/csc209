# Low-Level File I/O

## File I/O: File Descriptors

![Example of FDs pointing to inodes.](figures/file-descriptor.jpg){width=80%}

A file descriptor (FD) is a number (non-negative integer) that uniquely identifies an open file in a computer's operating system. It describes a data resource, and how that resource may be accessed.

[`www.computerhope.com/jargon/f/file-descriptor.htm`](https://www.computerhope.com/jargon/f/file-descriptor.htm)

## File I/O: Streams, File Descriptors & Pointers

Streams are files to which data is written or from which data is read. They're accessed through file pointers (`FILE *` from `<stdio.h>`) that wrap around FDs. The following default streams (FDs) are provided by `<stdio.h>`.

- `stdin` (0): default input; typically from user keyboard or pipes.
- `stdout` (1): default output; usually to terminal screen or pipes.
- `stderr` (2): default error; also to terminal screen.
- Use `>` to _redirect_ `stdout`, and `2>` to redirect `stderr`
  - `>` overwrites the output file, `>>` appends to it.

## File I/O: Opening & Closing

```c
FILE *fopen(const char *filename, const char * mode);
```

A file `filename` is opened with `fopen()` in a `mode` `{r|w|a}{|+}` to perform the following operations. Returns a file pointer that wraps the FD. The pointer is `NULL` if we fail to open the file (often because the file doesn't exist or your process doesn't have permission).

| action\\mode  | `r`  | `w`    | `a`    | `r+` | `w+`     | `a+`   |
| ------------- | ---- | ------ | ------ | ---- | -------- | ------ |
| read          | yes  | no     | no     | yes  | yes      | yes    |
| write         | no   | yes    | no     | yes  | yes      | yes    |
| append        | no   | no     | yes    | no   | no       | no     |
| file exists   | ok   | ok     | ok     | ok   | truncate | append |
| doesn't exist | fail | create | create | fail | ok       | create |

```c
int fclose(FILE *stream);
```

- `stream`: a `FILE *` opened by `fopen()`/`freopen()`.
- returns: `0` if closed properly, `EOF` otherwise.

## File I/O: Reading & Writing

### Reading

1. `getchar()`: read a character from `stdin`.
2. `fgetc()`: read a single character from the file.
3. `fgets()`: read strings from files.
4. `fscanf()`: formatted input from a file.
5. `fread()`: block of raw bytes from files; useful for binary files.

Examples: `e1.c`, `e2.c`.

### Writing

You can use `putchar()` to write a character to `stdout`.

```c
size_t fwrite(const void *ptr, size_t size,
    size_t nmemb, FILE *stream)
```

Alternatively, use `fwrite()` to write `nmemb` elements (each `size` large) from `*ptr` to `stream`.

## Low-Level I/O: A Deep Dive

The aforementioned library functions from `stdio.h` are high-level conveniences! But sometimes we need to work with lower-level functions. When we want to do low-level I/O, to bypass the buffering and abstractions provided by the C standard library, we must use _system calls_, namely:

1. `open()`
2. `close()`
3. `read()`
4. `write()`

Many different flags, errors, corner cases, etc. to consider.

- See `man 2 <sys-call>` for each.

## Low-Level I/O: Be Careful!

Proper usage of low-level I/O often requires looping and handling error conditions.

```c
ssize_t ret;
while (len != 0 && (ret = read(fd, buf, len)) != 0) {
    if (ret == -1) {
        if (errno == EINTR)
            continue;
        perror("read");
        break;
    }
    len -= ret;
    buf += ret;
}
```

## Low-Level I/O: File Descriptors

As before, we use _file descriptors (FDs)_, serving as indices for open files.

![from [`tinf2.vub.ac.be/~dvermeir/manuals/uintro/uintro.html`](https://tinf2.vub.ac.be/~dvermeir/manuals/uintro/uintro.html)](figures/filetable.gif){width=80%}

Each process has its own _file descriptor (FD) table_. File descriptor `N` in _process A_ can refer to a different file than file descriptor `N` in _process B_.

## Low-Level I/O: File Descriptors vs. Descriptions

![Mapping between file descriptors and file descriptions](figures/file_descriptors.svg){width=90%}

## Low-Level I/O: `exec` and File Descriptors

When a program calls `exec`, its process is replaced by a new program that still retains the FDs of the original process.

But upon replacing the process image with the new program, the `FILE *` variables are gone (memory leaks?).

So the new program must either:

1. Perform low-level I/O using the `read()` or `write()` system calls; or
2. Use `fdopen()` to associate a new buffered file stream with an existing open file descriptor.
   ```c
   FILE *fdopen(int fd, const char *mode);
   ```

More on `exec`/`execl` when we discuss processes in-depth.

## Low-Level I/O: `dup`/`dup2`

```c
int dup(int oldfd);
int dup2(int oldfd, int newfd);
```

- `dup` returns a new FD that refers to the same file as `oldfd`.
- `dup2` does the same, but lets you specify the value of new FD.
  - `dup2` first closes `newfd` if already in use.

### Output Redirection with dup2

```c
int main (void) {
    int fd = open("lsout", O_WRONLY | O_CREAT, 0600);
    if (fd == -1) {
        perror("open");
        exit(1); }
    dup2(fd, STDOUT_FILENO);
    execl("/bin/ls", "ls", "-l", (char *)NULL);
    perror("execl");
    return 1; // why are we always returning 1?
}
```

## Low-Level I/O: Arnold's Examples

`dup`: [`[www]/lectures/arnold/w07/lowLevelFileIO/lowLevelFileIO6.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/arnold/w07/lowLevelFileIO/lowLevelFileIO6.c)

`dup2`: [`[www]/lectures/arnold/w07/lowLevelFileIO/lowLevelFileIO7.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/arnold/w07/lowLevelFileIO/lowLevelFileIO7.c)

where `[www] = mcs.utm.utoronto.ca/~209/23s`
