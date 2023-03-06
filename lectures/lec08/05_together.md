# Putting it Together

## Putting it Together

A shell can `fork` and `exec` to execute other programs. For example...

1. Shell process `p` waits for keyboard input.
2. You type `ls`.
3. Shell forks child process `c`.
4. Process `c` uses an exec function to run `ls`.
5. Process `p` calls `wait` to wait for c to terminate, and then prints new prompt.

The skeleton might look like this:

```c
while (1) { // infinite
    print_prompt();
    read_command(command, parameters);
    if (fork()) wait(&status); // parent
    else execve(command, parameters, NULL); // children
}
```

## Putting it Together: Arnold's Examples (1)

```c
int main() {
    int x = fork();
    if (x == 0) { /* child */
        execl("/bin/ls", "ls", (char *)NULL);
        perror("/bin/ls");
        return 1;
    }
    /* parent */
    int status;
    wait(&status);
    printf("exit status %d\n", status >> 8);
    return 0;
}
```

## Putting it Together: Arnold's Examples (2)

```c
int main() {
    for (int num = 2; num > 0; num--) {
        if (fork() == 0) { /* child */
            execl("/bin/ls", "ls", (char *)NULL);
            perror("/bin/ls");  // should never get here!!
            exit(1);
        }
    }
    /* parent */
    int status;
    while (wait(&status) > 0)
        printf("Parent: exit status %d\n", WEXITSTATUS(status));
    return 0;
}
```

## Putting it Together: Arnold's Examples (3)

```c
int main() {
    if (fork() > 0) { /* parent */
        int status, pid = wait(&status);
        printf("pid %d exit status %d\n", pid, status >> 8);
        return 0;
    }
    /* child */
    close(1); // 1==stdout
    // NOTE: the next open takes the first open fd (=1).
    if (open("file", O_WRONLY|O_CREAT|O_TRUNC, 0666) < 0) {
        perror("file"); return 1;
    }
    // where is stdin coming from, stdout going to?
    // remember, command run by exec inherits all open fds!
    execl("/bin/ls", "ls", (char *)NULL);
    perror("/bin/ls"); return 1;
}
```

## Finale: Don't Try This At Home

**The University of Toronto will not be held liable for any data loss or system damage resulting from the use of the following program.**

```c
// C program Sample for FORK BOMB
// It is not recommended to run the program as
// it may make a system non-responsive.
#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"

int main() {
    while(1) fork();
    return 0;
}
```

Read about the fork bomb: [`www.geeksforgeeks.org/fork-bomb/`](https://www.geeksforgeeks.org/fork-bomb/)
