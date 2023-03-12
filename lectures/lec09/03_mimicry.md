# Practice by Mimicry

One way to learn is by imitating or mimicking something/someone else. Consider the following Python code:

```py
from sys import argv # this is analogous to the int **argv in C
argslen = sum([len_verbose(arg) for arg in argv])
print(f"The length of all the args is {argslen}")
```

As a quick review, put this code in a file called `argslens` and make it executable with `python`. See if you remember how!

## Practice by Mimicry

One way to learn is by imitating or mimicking something/someone else. Consider the following Python code:

```py
from sys import argv # this is analogous to the int **argv in C
argslen = sum([len_verbose(arg) for arg in argv])
print(f"The length of all the args is {argslen}")
```

As a quick review, put this code in a file called `argslens` and make it executable with `python`. See if you remember how!

1. Find `python` (`which python`) and prepend a shebang: `#!/usr/bin/python`
2. Make it executable: `chmod a+x argslens`

Now rewrite it in C. Use `fork()` and `pipe()` to mimic this behaviour such that each child process handles one argument.

### Karen's Solutions

[`github.com/rhubarbwu/csc209/blob/master/lectures/lec09/argslens.c`](https://github.com/rhubarbwu/csc209/blob/master/lectures/lec09/argslens.c)

## Practice by Mimicry

Now, try writing programs using C pipes to mimic UNIX pipes.

```sh
$ sort -n < infile > outfile
$ sort -n | tail -3
$ sort -n < infile | tail -3
$ sort -n < infile | tail -3 > outfile
$ head -10 < infile | sort -n | tail -3 > outfile
```

Hint: each UNIX pipe `|` or redirection `<`/`>` might involve reading/writing of some file descriptor. And each separate command might involve calling `fork()`/`exec*()`.

Remember that UNIX pipes are simpler: they only deal with input and output of whole commands and not their subprocesses. On the other hand, mimicking them with C still requires a fair bit of code...

### Arnold's Solutions

[`www.cs.toronto.edu/~arnold/209/12s/lectures/pipesAndRedirection`](http://www.cs.toronto.edu/~arnold/209/12s/lectures/pipesAndRedirection/index.html)
