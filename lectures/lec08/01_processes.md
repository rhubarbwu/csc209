# Processes

## Processes: What are they?

A **program** is executable file on disk (either source code or compiled machine code), and a **process** is a running instance of a program.

- Executing multiple instances of the same program launches multiple processes.
  - Example: running multiple instances of Notepad.
- A single instance of a program may launch multiple processes.
  - Example: Firefox/Chrome run one-process-per-tab.
- Each process has its own memory space, including its own stack and heap.
- A process cannot access the variables/memory of another process.
- Process ID (PID): unique, non-negative integer identifier; a handle by which to refer to a process.
- The first process created when the system boots up is `init`, with PID `1`.
  - `init` is typically provided by `systemd` on Linux.
  - Others such as `runit` and `openrc` exist for Linux/BSD/UNIX-like systems.
  - On MacOS, the `init` process is `launchd`.

## Processes: What do they do?

Processes can be running, ready or blocked.

![processes grouped by states](lec08/proc1.jpg){width=80%}

### Currently-Running Processes

Try `pstree` from `bash` to print the tree of currently-running processes

## Processes: What do they do?

Processes can be running, ready or blocked.

![what the states mean](lec08/proc2.jpg){width=80%}

### Currently-Running Processes

Try `pstree` from `bash` to print the tree of currently-running processes

## Processes: Creation

In UNIX-like systems, processes are created by the `fork()` system call.

- `fork()` allows one process, the parent, to create a new process, the child.
- The new child process is an (almost) exact duplicate of the parent: The two processes execute the same program text and the child obtains copies of the parent’s stack, data, and heap. Their PIDs are different though.
- After `fork()` has completed its execution,
  - two processes exist, and, in each process, execution continues from the point where `fork()` returns.
  - each process can modify the variables in its stack, data, and heap segments without affecting the other process;
  - however, we don’t know whether the parent process or the child process will execute first.
- The two processes can be distinguished via the value returned from `fork():`
  - Return value in parent is the PID of the child process.
  - Return value in child is 0 if there was no error.

## Processes: `pid_t` (data type)

The `pid_t` data type is a signed integer type which is capable of representing a PID. In the GNU C Library, this is an `int` (but could be `long`\*). Consider the following.

```c
int i = 5;
printf("%d\n", i);
pid_t pid = fork();
if (pid > 0)
    i = 6; /* only parent gets here */
else if (pid == 0) {
    i = 4; /* only child gets here */
    printf(" child: %d\n", pid); // 0, not the actual PID
    printf("parent: %d\n", ???); // parent's PID?
}
printf("%d\n", i);
```

- How to print the actual PID of the child if `pid` is `0`?
- How to print the PID of the parent from the child process?

## Processes: `pid_t` (functions)

How to print the actual PID of the child if `pid` is `0`? How to print the PID of the parent from the child process?

We have access to a few functions that return `pid_t` identifiers.

```c
pid_t getpid (void);  // returns the PID of the current process.
pid_t getppid (void); // returns the parent PID of the current process.
pid_t gettid (void);  // returns the thread ID of the current thread
```

We can then call `getpid` and `getppid` in the child process.

```c
else if (pid == 0) {
    i = 4; /* only child gets here */
    printf(" child: %d\n", getpid());
    printf("parent: %d\n", getppid());
}
```

## Processes: Creation (Example)

What happens when you fork? Conceptually, it looks like this:

```c
// original process (parent)    // child process
int i = 5;
printf("%d\n", i);
pid_t pid = fork();             pid_t pid = fork(); // spawns
if (pid > 0)                    if (pid > 0)
    i = 6;                          /* doesn't run */
else if (pid == 0) {            else if (pid == 0) {
    /* doesn't run */               i = 4;
                                    printf("child: %d\n", getpid());
                                    printf("parent: %d\n", getppid());
}                               }
printf("%d\n", i);              printf("%d\n", i);
```

## Processes: Monitoring

In many applications where a parent creates child processes, it is useful for the parent to be able to monitor the children to find out when and how they terminate. This facility is provided by `wait()` and a number of related system calls.

```c
#include "sys/wait.h"
pid_t wait(int *status);
```

The `wait()` system call waits for one of the children of the calling process to terminate and returns the termination `status` of that child in the buffer pointed to by `status`. Returns PID of terminated child, or `-1` on error.

- If no (previously unwaited-for) child of the calling process has not yet terminated, the call blocks until one of the children terminates.
- If a child has already terminated by the time of the call, `wait()` returns immediately, returning the process ID of the child that has terminated. The termination `status` of the child is stored at the memory space `status` is pointing to.
- If the calling process doesn’t have any child processes, `wait()` returns immediately with an error.

## File Descriptors vs. File Descriptions

![Mapping between file descriptors and file descriptions](lec07/file_descriptors.svg){width=85%}

File descriptors in different processes may refer to the same _open file description_ in OS's _open file table_.

## File Descriptors: Sharing the Same Open File Description

A child process gets a **copy** of the parent's file descriptor table. So all open files open in the parent **before** the `fork` are also open in the child.

Immediately after the `fork`, both parent and child's file descriptors refer to the same _open file descriptions_ in the system-wide _open file table_.

If the parent process `open`s a file before a `fork`:

- Whichever process (parent or child) calls `fread` first
  will read the first `N` bytes.
- Whichever process calls `fread` afterwards will read
  the next `M` bytes.

Since both parent and child processes refer
to the same file description, they both share the
same _offset_ value.
