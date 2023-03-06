# UNIX & Filesystems

## UNIX: What is it? $(\beta)$

![The UNIX Timeline](figures/unix_timeline.svg){width=80%}

## UNIX: Philosophy $(\beta)$

Brief summary of the [UNIX philosophy](https://en.wikipedia.org/wiki/Unix_philosophy), from _A Quarter-Century of UNIX_ by P. H. Salus, 1994:

- Write programs that do one thing and do it well.
- Write programs to work together.
  - Expect that the output from your program will be used as input for another (e.g., by piping).
  - Don't require interactive input.
- Write programs that handle text streams, because that is a universal interface.

## UNIX: Implications of this Philosophy

UNIX programs

- input: files, args, signals, environment vars
- output: files, return code, signals, environment vars
- defaults: stdin, stdout, stderr, args, environment

When writing a unix command:

- If you have no filename arg, then take `stdin`.
- If no file name argument for output, print to `stdout`.
- Behave as if you can take many files.
- Print out so that other programs can understand you
- Return an appropriate return code.

## Files: What are they?

- Files are objects that contain information.
- Everything is a file!
  - Directories are files too.
  - Hidden files a prefixed with a dot `.`, like `.gitignore` or `.ssh/`.
  - Symbolic links are references to other files.

## Files: What is a Directory? $(\beta)$

- A directory is a file that contains _directory entries_.
- Directory entries map file names to _inode_ numbers.
- An inode is a data structure of information about a file, such as its:
  - Access permissions
  - Size
  - Physical location on disk
- More about inodes in CSC369...

## Files: Directories, Paths & Hierarchy

Components of a path are joined with `/`.

### Absolute Paths

In actuality, files have absolute paths stemming from the root.

- `/usr/bin/python`
- `/home/rupert/209h5s`

### Relative Paths

- Files can mostly be used with their name:
  - `here.txt`
- More reliably with `.`:
  - `./definitely-here.txt`
- To refer up a directory, prepend `..`:
  - Sibling file: `../sibling.txt`
  - Cousin file: `../../other/cousin.txt`

## Files: Permissions ($\beta$)

![file permissions](figures/file_perms.svg)

- Each file has a permission string, e.g., `rw-r-xr-x`
  - First column: `d` (directory), `l` (link), or `-` (regular file)
  - `rwx` flags represent _read_, _write_, &; _execute_ permissions
  - Permissions for the owning _user_ & _group_ and _other_ users.

### Directories

`r` allows listing the directory's contents (`ls`), `w` allows creating/deleting directory's entries, `x` allows entering the directory (`cd`)

### Example

Visit this: [`www.cs.toronto.edu/~rupert/xmonad.png`](www.cs.toronto.edu/~rupert/xmonad.png)

## Files: An Analysis ($\beta$)

```python
$ ls -la
total 16
drwxr-xr-x 4 bob staff 4096 Jan  6 20:18 .
drwxr-xr-x 3 bob staff 4096 Jan  6 20:18 ..
-rw-r--r-- 1 bob staff    0 Jan  6 20:16 file1
lrwxrwxrwx 1 bob staff    5 Jan  6 20:17 file2 -> file1
-rw-r--r-- 1 bob staff    0 Jan  6 20:18 .hidden
drwxr-xr-x 2 bob staff 4096 Jan  6 20:16 test1
drwxr-xr-x 2 bob staff 8192 Jan  6 20:16 test2
$
```

- From left to right: file permissions, link count, owning user, owning group, file size, last modified date, and file name (symbolic link indicated by `->`).
- `ls -ali` shows inode numbers in the first column.

## Files: Common Commands

| command          | the gist                                   |
| ---------------- | ------------------------------------------ |
| `cd`             | changes working directory                  |
| `ls`             | lists files in a directory                 |
| `touch`          | refreshes a file                           |
| `mkdir`          | creates directories                        |
| `ln`             | creates hard or symbolic links             |
| `cp`, `mv`       | copy or move files (or directories)        |
| `chmod`, `chown` | change permissions or owner/group of files |

### Changing File Permissions

- Modify for `{u|g|o|a}{+|-}{r|w|x}`
  - `+` (add) or `-` (remove) permissions
  - `u` (owner), `g` (group), `o` (others), `a` (all)
- Set permissions in octal notation
  - add up `r` (4?), `w` (2?), and `x` (1?): `[0-7][0-7][0-7]`

## Files: Streams & Redirection $(\beta)$

- Standard I/O streams that every process starts with:
  - `stdin`: By default, reads input from keyboard
  - `stdout`: By default, writes to the console display
  - `stderr`: By default, writes to the console display
- The process treats these streams as files (surprise!)
- Use `>` to _redirect_ `stdout`, and `2>` to redirect `stderr`
  - `>` overwrites the output file, `>>` appends
  ```sh
  $ ls >myfiles.txt
  ```

[`linux.die.net/Intro-Linux/sect_05_01.html`](https://linux.die.net/Intro-Linux/sect_05_01.html)

## Files: Pipes & Process Substitution $(\beta)$

- _Pipes_ transfer output from one process to another
  ```sh
  $ ls | grep "pdf"
  ```
- _Input redirection_ transfers the contents of a file into `stdin` of a process
  ```sh
  $ wc <essay.txt
  ```
- _Process substitution_ creates a _temporary file_ to transfer the output from one or more processes to `stdin` of another process
  ```sh
  $ wc <(ls) or wc <(ls | grep "pdf")
  ```

[`http://tldp.org/LDP/abs/html/process-sub.html`](http://tldp.org/LDP/abs/html/process-sub.html)
