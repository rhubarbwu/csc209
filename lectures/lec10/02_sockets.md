# Sockets

## Pipes: Unidirectional vs. Bidirectional Communication

![](lec10/pipes.svg)

## Sockets

![](lec10/sockets.svg)

- A server must have a _listener socket_ to accept new connections
- A separate socket is created to communicate with each client

## Sockets: System Calls for Setting up a Server

```c
#include "sys/socket.h"
int socket(int family, int type, int protocol);
int setsockopt(int sockfd, int level, int optname,  const void *optval.
                    socklen_t optlen);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *restrict addr,
                    socklen_t *restrict addrlen);
```

A server is typically created and used like this...

1. `socket`: Creates a socket; configure with `setsockopt`.
2. `bind`: Assigns an address and port to the socket. (must assign an IP address that actually belongs to your systems).
3. `listen`: Establish a queue for incoming connections.
4. `accept`: Accept queued incoming connection and create a new socket.
5. `read`/`write` (or `recv`/`send`): Receive/send data on socket.
6. `close`: Close resources and the socket itself.

## Sockets: System Calls for Setting up a Client

```c
#include "sys/socket.h"
int socket(int family, int type, int protocol);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// analogs of read()/write() that take flags
ssize_t recv(int __fd, void *__buf, size_t __n, int __flags);
ssize_t send(int __fd, const void *__buf, size_t __n, int __flags);
```

An accompanying client can be created to connect to such a server...

1. `socket`: Creates a socket; configure with `setsockopt`.
2. `connect`: Connects to a remote server using an IP address and port.
3. `read`/`write` (or `recv`/`send`): Receive/send data on socket.
4. `close`: Close resources and the socket itself.

### Look familiar?

Recall the system calls `read`/`write`/`close` from `unistd.h`; you use them here just like with regular files and pipes! `recv`/`send` are analogs of `read`/`write` provided by `sys/socket.h` that can take flags (bit vector `int __flags`). See `man` pages!

## Sockets: The Server-Client Model

![](lec10/socket_syscalls.svg)

## Sockets: Identifying & Multiplexing

A socket can be identified by its source/destination addresses/ports, and its protocol.

```rust
{SRC-IP, SRC-PORT, DEST-IP, DEST-PORT, PROTOCOL}
```

This uniqueness allows some useful properties.

- A server or client can have multiple sockets, connecting to clients/servers.
- Multiple clients can connect to the same server socket, and it's up to the server to dispatch child processses (or threads) to process them.
  - This is how a shared machine can be used by multiple users at once.

### What the community says...

[`stackoverflow.com/questions/3329641/how-do-multiple-clients-connect-simultaneously-to-one-port-say-80-on-a-server`](https://stackoverflow.com/questions/3329641/how-do-multiple-clients-connect-simultaneously-to-one-port-say-80-on-a-server)

## Sockets: IP Addresses

System calls expect the IP address to be passed in as an `in_addr` struct.

\small

```c
#include "sys/socket.h"
struct in_addr { uint32_t s_addr; }; // 32 bits/4 bytes
struct in6_addr { unsigned char s6_addr[16]; }; // how big is this?
```

\normalsize

There are system calls to help you convert back and forth between `in_addr` structs and human-readable strings (`a.b.c`).

\small

```c
#include "arpa/inet.h"
int inet_aton(const char *cp, struct in_addr *inp); // a.b.c -> struct in_addr
char *inet_ntoa(struct in_addr in); // in_addr -> a.b.c
```

\normalsize

To support IPv6, there are analogs `inet_pton`/`inet_ntop`.

\small

```c
#include "arpa/inet.h"
int inet_pton(int af, const char *src, void *dst);
const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
```

\normalsize

The latter is favoured over the deprecated `inet_ntoa`. See `man 3 inet_ntop`.

## Sockets: Socket Addresses for IPv4

Socket addresses contain 16 bytes, mostly for the protocol address.

\small

```c
#include "sys/socket.h"
struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
```

\normalsize

However, when interfacing with IPv4 or IPv6, we only need to know the `AF_INET`, internet address and port number (which are part of the protocol address)!

\small

```c
// (IPv4 only--see struct sockaddr_in6 for IPv6)
struct sockaddr_in {
    short int          sin_family;  // address family, AF_INET
    unsigned short int sin_port;    // port number
    struct in_addr     sin_addr;    // internet address
    unsigned char      sin_zero[8]; // padded to sizeof(struct sockaddr)
};
```

\normalsize

But if `connect()` requires a `sockaddr`, how can we use `sockaddr_in`? They're the same size so you can cast either to the other.

## Sockets: Socket Addresses for IPv6

For IPv6, we use `sockaddr_in6`. How big is this?

```c
#include "sys/socket.h"
// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)
struct sockaddr_in6 {
    u_int16_t       sin6_family;   // address family, AF_INET6
    u_int16_t       sin6_port;     // port number, Network Byte Order
    u_int32_t       sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr;     // IPv6 address
    u_int32_t       sin6_scope_id; // Scope ID
};
```

If we want portable code, we'd abstract socket addresses for both IPv4 and IPv6 for as much interaction as possible. For that, there is the `sockaddr_storage` struct, a union-like that is padded/aligned to be cast-compatible with both IPv4 and IPv6.

### Further Reading: Beej's Guide to Network Programming (3.3)

\small [`beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html`](https://beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html)

## Sockets: Incomplete Messages & Boundaries

Messages aren't always perfect (i.e. incomplete), and sometimes there are many of them to process.

Suppose that a sender sends a sequence of bytes to a receiver over a TCP socket.

- TCP guarantees that the receiver will receive the entire sequence, eventually.
- But it's possible that when the receiver calls `read()` on the socket:
  - The entire message wasn't received yet.
  - The `read()` call was interrupted (e.g., see `EINTR` in `man 2 read`).

You would somehow need to know the correct sequence length (in other words, a known message boundary) to wait for or count up to. What is "correct" could be standardized, or declared in a prefix of the sequence.

## Sockets: Buffering

What if you receive more messages faster than you're able to handle?

- Buffering is an extremely common technique, especially in networking.
- The operating system does its own buffering, but programmers typically also use buffering in many areas of systems/network/web programming.

## Sockets: Demo & Beyond

Arnold provided a server-client example where clients send text to the server, which returns it back with alphabetical letters upper-cased.

[`mcs.utm.utoronto.ca/~209/23s/lectures/src/sockets/server.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/sockets/server.c)
[`mcs.utm.utoronto.ca/~209/23s/lectures/src/sockets/client.c`](https://mcs.utm.utoronto.ca/~209/23s/lectures/src/sockets/client.c)

Compile the server and client separately.

```c
$ gcc -o server server.c
$ gcc -o client client.c
```

Then start the server and connect a client.

```c
$ ./server &
$ ./client
```

Notice that the server is treating each client individually/independently...

### What's next? Multiplexing & `select`

Can we build useful multi-user programs like file-sharing or chat applications? Not so fast! We'll discuss `select` and I/O multiplexing next week!
