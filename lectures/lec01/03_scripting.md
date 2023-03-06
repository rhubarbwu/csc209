# Shell Scripting

## Scripting: Shells

An interpreter application that invokes commands and runs scripts.

- `bash` and `zsh` are commonly found in UNIX-like systems.
- They all have their own syntax and implementations.
- POSIX is a standard and we always prefer POSIX-compliant shells.
  - See more at [`itsfoss.com/posix/`](https://itsfoss.com/posix/).
- We will use `bash`.

## Scripting: Variables & `PATH`

Variables are declared with the `=` operator and referenced with `$`.

```sh
$ course=csc209
$ echo $course
csc209
```

### Special Variables

- `$?` returns the _exit code_ of the last function/command the shell ran.
- `$PATH` gives `:`-separated list of paths the shell searches for binaries in.
- `$0` is the name of the current script/function/command/application.
- `$1`, `$2`, ... are the arguments to your function/script.

## Scripting: Strings

1. Single Quote `'`:

- All special characters between these quotes lose their special meaning.

2. Double Quote `"`:

- Most special characters between these quotes lose their special meaning with these exceptions: `$`, ` \``,  `\$`, `\'`, `\"`, `\\`

3. Backslash `\`:

- Any character immediately following the backslash loses its special meaning.

4. Back quote <kbd>`</kbd>:

- Anything in between back quotes would be treated as a command and would be executed.

## Scripting: Evaluations & Conditions

`$(cmd)` aka <kbd>\`cmd\`</kbd> executes `cmd` in shell and is replaced by the result.

- `$((<expr>))` allows you to perform algebra on `<expr>`.

  [`mywiki.wooledge.org/ArithmeticExpression`](https://mywiki.wooledge.org/ArithmeticExpression)

### Conditions - `test`

- Conditions can take the form of `[ <expr> ]`.
  - `bash` is more flexible/powerful and supports two-layer brackets `[[ ]]` and/or double equals `==` that have other features.
- Alternatively, you can use `test <expr>`.
- Conditions can be negated with `!`.
- Conditions can be based on (unary) flags or (binary) comparisons.
- A disjunction ("or") of conditions can be strung with `-o` or `||`.
- Likewise, a conjunction ("and") can be strung with `-a` or `&&`.
- More about conditions next week...

[`www.geeksforgeeks.org/shell-scripting-test-command/`](https://www.geeksforgeeks.org/shell-scripting-test-command/)

## Scripting: If/Else

Shell supports `if`/`elif`/`else` and `while` like this...

```sh
if $condA; then
  echo A
elif $condB; then
  echo B
else; then
  echo neither
fi

while $condC; do
  echo C
done
```

## Scripting: For Loops

- You can iterate over a static list...
  ```sh
  for a day in Fri Thu Wed Tue Mon; do
    echo “Todays day is $day”
  done
  ```
- Or over a list stored in a variables...
  ```sh
  Month = “Jan Feb Mar Apr May Jun”
  for mon in $Month; do
    echo “Month is $mon”
  done
  ```
- Or take command-line arguments
  ```sh
  for num; do echo “Number is $num”; done
  ```

[`www.educba.com/for-loop-in-shell-scripting/`](https://www.educba.com/for-loop-in-shell-scripting/)

## Scripting: Functions

Putting the pieces together in a function...

```sh
is_div_six() {
   x=$1
   two=$((x % 2))
   if [ $((x % 3)) = 0 ] && test $two = 0; then
      echo "yes"
   elif test $((x % 3)) -eq 0; then
      echo "three"
   elif [ ! $two -eq 0 ]; then
      echo "no"
   else echo "two"; fi
}
is_div_six 12 # yes
is_div_six 64 # two
is_div_six 39 # three
```

## Scripting: Globbing $(\beta)$

- _Globbing_ patterns are strings that expand to match multiple file names
  - Similar, but simpler, than regex: see `man 7 glob`
- `?` matches any single character
- `*` matches any string, including the empty string
- `[list of characters]` matches a single character inside the list, e.g., `[abc]`
- Usage examples:
  - `rm \*.log`: Remove all files ending in `.log`
  - `ls \*.pdf`: List files ending in `.pdf`

## Scripting: Shebang & Executables

To make files executable, use `chmod` to change the permissions to allow it.

- Executables can be run with filepath.
- If an executable is found in `$PATH`, it can be run just by name.

Interpreters such as shells can be specified with shebang `#!` at the top of the file. Otherwise, the inherent shell will be used.

```sh
#!/usr/bin/dash
$ echo this is dash
```

You can do the same with Python.

```python
#!/usr/bin/python
print("this is python")
```
