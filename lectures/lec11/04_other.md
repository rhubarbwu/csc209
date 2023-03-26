# More On Multiplexing

## Reading From Clients

When a server does a `read()`, it is not guaranteed to receive a complete line or all of the desired bytes. For example:

- The client could be sending each character separately.
- The client could send data that gets split over several segments.

Want to operate only on full lines? The server must keep each partial line in a buffer until it gets the newline from the client.

### Buffering for Full Lines

The following code assumes there's at most one line in the buffer.

\small

```c
struct client {
    int fd;
    char buf[300];
    int inbuf;
    struct client *next;
};
```

\normalsize

The server should keep a buffer for each client, and keep track of the number of bytes in each buffer following the previous message.

## Buffering for Full Lines...

Read bytes, check for errors, and null-terminate the string.

\small

```c
void myread(struct client *p) {
    int room = sizeof(p->buf) - p->inbuf;
    if (room <= 1) {} // clean up this client: buffer full

    char *startbuf = p->buf + p->inbuf;
    char *tok, *cr, *lf;
    int crlf;

    int len = read(p->fd, startbuf, room - 1);
    if (len <= 0) {} // clean up this client: eof or error

    p->inbuf += len;
    p->buf[p->inbuf] = '\0';
    ...
}
```

Making sure to start at the point up to which the buffer's filled.

## Buffering for Full Lines...

If a full line exists, process it and shift it out of the buffer.

\small

```c
void myread(struct client *p) {
    ...
    lf = strchr(p->buf, '\n');
    cr = strchr(p->buf, '\r');
    if (!lf && !cr) return; // no complete line

    tok = strtok(p->buf, "\r\n");
    if (tok) {} // use tok (complete string)

    // compute how many bytes we're removing
    if (!lf) crlf = cr - p->buf;
    else if (!cr) crlf = lf - p->buf;
    else crlf = ((lf > cr) ? lf : cr) - p->buf;
    crlf++; // include the CRLF

    p->inbuf -= crlf; // shft the remainder towards the head
    memmove(p->buf, p->buf + crlf, p->inbuf);
}
```

## `SIGPIPE`

Suppose you're writing to a broken pipe/socket generates a `SIGPIPE`. By default, most signals (including `sigpipe`) will terminate your program. Here's how you can protect against `sigpipe`:

```c
/*
 * Turn off SIGPIPE: write() to a socket that
 * is closed on the other end will return -1
 * with errno set to EPIPE, instead of generating
 * a SIGPIPE signal that terminates the process.
 */
if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    perror("signal");
    exit(1);
}
```

## Non-Blocking Reads

You can change the behaviour of `read()` so that it returns `-1` and sets `errno` to `EAGAIN` if no data is available.

- In this mode, `read()` will never block.
- Downside is that it will lead to inefficient code, e.g., using an infinite loop that repeatedly calls `read()`.
  - Remember, `read()` will return **immediately** in non-blocking mode, so you will be calling it **many** times per second.

## Non-Blocking Reads: Sample Code

\small

```c
char buf[1024];
ssize_t bytesread;
/* set O_NONBLOCK flags on fd1 and fd2 */
if (fcntl(fd1, F_SETFL, O_NONBLOCK) == -1) perror("fcntl"); exit(1);
if (fcntl(fd2, F_SETFL, O_NONBLOCK) == -1) perror("fcntl"); exit(1);

for (;;) {
    bytesread = read(fd1, buf, sizeof(buf));
    if ((bytesread == -1) && (errno != EAGAIN))
        return; // real error
    else if (bytesread > 0)
        doSomething(buf, bytesread);

    bytesread = read(fd2, buf, sizeof(buf));
    if ((bytesread == -1) && (errno != EAGAIN))
        return; // real error
    else if (bytesread > 0)
        doSomething(buf, bytesread);
}
```
