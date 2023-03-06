# Processes: Test Your Understanding

## Processes: Test Your Understanding

1. How do we obtain the program exit status from the process exit status?
2. What happens if a process terminates before its exit status is obtained by its parent?
3. What happens if a parent process terminates before waiting for all its children?

## Processes: Test Your Understanding

Here are some questions:

1. How do we obtain the program exit status from the process exit status?
   - Use macros defined in `wait.h` (see `man 2 wait`), e.g.,
   - `WIFEXITED(status)` to see if process terminated normally or abnormally.
   - `WEXITSTATUS(status)` to obtain program exit status.
2. What happens if a process terminates before its exit status is obtained by its parent?
   - The child process becomes a _zombie process_. Operating system retains minimal information about the process until the parent obtains exit status via `wait()`.
3. What happens if a parent process terminates before waiting for all its children?
   - The child processes become _orphan processes_. Orphan processes are _adopted_ by the `init` process.

## Processes: Test Your Understanding

Which statements are true when `fork()` is called?

1. True/False? The child process shares the same PID as the parent process.
2. True/False? `fork()` will only fail if the total number of processes under execution by a single user would be exceeded.
3. True/False? After `fork()` has been called successfully, a value of 0 is returned to the child.

## Processes: Test Your Understanding

Which statements are true when `fork()` is called?

1. True/False? The child process shares the same PID as the parent process. **False**.
2. True/False? `fork()` will only fail if the total number of processes under execution by a single user would be exceeded. **False**.
3. True/False? After `fork()` has been called successfully, a value of 0 is returned to the child. **True**.

See `man 2 fork`: [`linux.die.net/man/2/fork`](https://linux.die.net/man/2/fork)

## Processes: Test Your Understanding

\setstretch{1.0}

Consider the program below that runs without errors. How many child processes?

```c
int main(void) {
    printf("Mangoes\n");
    int r = fork();
    printf("Apples\n");
    if (r == 0) {
        printf("Oranges\n");
        if (fork() >= 0) printf("Bananas\n");
        return 0;
    }
    printf("Peaches\n");
    for (int i = 0; i < 3; i++) {
        if ((r = fork()) == 0) {
            printf("Pears\n"); exit(0);
            printf("Nectarines\n"); continue;
        }
        printf("Plums\n");
    }
    return 0;
}
```

## Processes: Test Your Understanding

\setstretch{1.0}

1. How many times is each fruit printed?

   | Fruit Name   | Times Printed |
   | ------------ | ------------- |
   | `Mangoes`    |               |
   | `Apples`     |               |
   | `Oranges`    |               |
   | `Bananas`    |               |
   | `Peaches`    |               |
   | `Pears`      |               |
   | `Nectarines` |               |
   | `Plums`      |               |

2. Several orderings of the fruit names are possible valid output. Some of these orderings even have the unix prompt displaying before the final fruit name (or names). Explain why this happens.
3. Not all of the fruit names could appear after the prompt in a valid output. For example the word Mangoes will never appear after the prompt. List all the fruit names that could occur after the prompt.

## Processes: Test Your Understanding

\setstretch{1.0}

1. How many times is each fruit printed?

   | Fruit Name                    | Times Printed |
   | ----------------------------- | ------------- |
   | `Nectarines`                  | 0             |
   | `Mangoes`/`Oranges`/`Peaches` | 1             |
   | `Apples`/`Bananas`            | 2             |
   | `Plums`/`Pears`               | 3             |

2. Several orderings of the fruit names are possible valid output. Some of these orderings even have the unix prompt displaying before the final fruit name (or names). Explain why this happens.

   - _The shell is the parent of and waits for the original process before printing the shell prompt. But this original process doesn't wait for its own children, so latters' output may be printed after their parents terminate._

3. Not all of the fruit names could appear after the prompt in a valid output. For example the word Mangoes will never appear after the prompt. List all the fruit names that could occur after the prompt.

   - `Apples`, `Bananas`, `Oranges`, `Pears`
