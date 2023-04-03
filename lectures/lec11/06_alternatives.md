
# Multiplexing I/O Alternatives

## Multiplexing I/O Alternatives: `poll()`/`ppoll()`

Although portable, `select()` has some performance limitations, and can only monitor at most `FD_SETSIZE` (1024, on Linux) FDs. Consider `poll()`/`ppoll()`.

```c
#include "poll.h"
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
int ppoll(struct pollfd *fds, nfds_t nfds, // analog to pselect()
        const struct timespec *tmo_p, const sigset_t *sigmask);
```

`select()` iterates over the array of all possible FDs (up to `nfds`) while only the active FDs are polled by `poll()`. By definition, $\mathcal O({\tt poll}) \leq \mathcal O({\tt select})$ and in most cases $\mathcal O({\tt poll}) \ll \mathcal O({\tt select})$ because most FDs between `0` and `nfds` are inactive.

### Portability & Modernity

Nowadays, `poll()` is widespread enough that it's considered the new portable standard. In other iterations of CSC209, `poll()` might be taught instead.

### Can we do better?

Some OS-specific system calls perform even better but are less portable. Conditional directives `#ifdef`/`#else`/`#endif` can sometimes offer a workaround.

## Multiplexing Alternatives: More Efficient, Less Portable (`epoll` on Linux)

`epoll()` is an API that performs a similar task to `poll()` but is much more scalable ($\mathcal O(1)$). It's both level- and edge-triggered.

\small

```c
#include "sys/epoll.h"
int epoll_create(int size);
int epoll_create1(int flags);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,
              int maxevents, int timeout);
int epoll_pwait(int epfd, struct epoll_event *events,
              int maxevents, int timeout,
              const sigset_t *sigmask);
int epoll_pwait2(int epfd, struct epoll_event *events,
              int maxevents, const struct timespec *timeout,
              const sigset_t *sigmask);
```

### Variations of `epoll()` - `libevent`, `libev`, ...

- [`libevent.org/`](https://libevent.org/)
- [`software.schmorp.de/pkg/libev.html`](http://software.schmorp.de/pkg/libev.html)

There are probably more...

## Multiplexing Alternatives: More Efficient, Less Portable (`kqueue` on BSD)

`kqueue()` also scales well ($\mathcal O(1)$) on BSD/MacOS.

\small

```c
#include "sys/event.h"
kqueue(void);
kevent(int kq, const struct kevent *changelist, int nchanges,
         struct kevent *eventlist, int nevents,
         const struct timespec *timeout);
EV_SET(&kev, ident, filter, flags, fflags, data, udata);

struct kevent {
    uintptr_t   ident;    /* identifier for this event */
    short	    filter;	    /* filter for event */
    u_short	    flags;    /* action flags for kqueue */
    u_int	    fflags;	    /* filter flag value */
    int64_t	    data;	    /* filter data value */
    void	   *udata;	    /* opaque user data identifier */
};
```

[`man.openbsd.org/kqueue.2`](https://man.openbsd.org/kqueue.2)
