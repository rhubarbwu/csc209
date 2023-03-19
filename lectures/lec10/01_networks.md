# Communication & Networks

## Communication: Humans vs. Computers

Communication can take many forms...

### Human Communication

Human communication is governed by syntactic rules: vocabulary, sentence structure, spelling, grammar...which form semantics and context.

```
How would you like it if a l l th elect ur e sli des we r e
fo r ma ttt ed like thiiiiiis ?!.$
```

### Computer Communication

Computer communication is also governed by rules:

- _Encoding_ of information, e.g., text, signed/unsigned integers, floating point
- _Ordering_ of bytes, e.g., big endian, little endian
- _Message sequencing_, e.g., first send username, then send password
- _Message start and end boundaries_, e.g., `CRLF` (`\r\n`) to terminate messages

## Communication: Newlines

Depending on your system, "lines" of data are separated differently.

- `LF` (`\n` or "line feed")
  - Equivalent to `^J` where `^` is the "control" key.
  - UNIX-like systems use `LF` to end lines.
- `CRLF` ("carriage return" + "line feed")
  - Equivalent to `^M` followed by `^J`.
  - Microsoft operating systems (such as DOS and Windows) end lines with `CRLF`.
- `CR`, `LF` could also take the form of 13, 10 (ASCII), or `\015`, `\012` (octal).

Because the vast majority of servers (back in the day) and general-purpose computers ran Microsoft operating systems, the `CRLF` is the adopted convention.

### Conversion

Many applications and network interfaces implicitly convert, but you can use `unix2dos` and `dos2unix` to convert files between the two newline conventions.

### Deeper Reading: Alan J. Rosenthal's CSC209

[`www.teach.cs.toronto.edu/~ajr/209/notes/sockets/newline.html`](https://www.teach.cs.toronto.edu/~ajr/209/notes/sockets/newline.html)

## Networks: Transport Protocols

Two widespread models of _transport protocols_ for
computer communication over a network:

1. Connectionless: Exemplified by `UDP` protocol
2. Connection-oriented: Exemplified by `TCP` protocol

Protocols are a set of _rules_.
Both TCP and UDP protocols are implemented by the _operating system_.

## Networks: User Datagram Protocol (UDP)

- UDP is used for sending a _datagram_ from one machine to another
- A datagram is a self-contained message with a beginning
  and end.
- The OS sends the datagram, but doesn't follow up to make sure that it got delivered.
- UDP is connectionless.

## Networks: Transmission Control Protocol (TCP)

- TCP is used to establish a _socket_ (similar to a pipe)
  to communicate between two processes.
  - Processes may be on the same computer, or two different computers
    connected by a network.
- The socket is created using a system call.
- The process sending the data writes a sequence of bytes to the socket.
- The OS guarantees that the bytes will be delivered over the network, in the correct order, to the receiving process.
- TCP is connection-oriented.

## Networks: UDP vs. TCP

- Comparing UDP and TCP is like comparing SMS and WhatsApp.
- If you send an SMS to your friend, you have no way of knowing if
  they received your message.
  - Perhaps they may reply back to you confirming that they received
    your message.
- If you send a message over WhatsApp, **the app itself tells you whether
  or not the message was successfully delivered**.

## Networks: Classic Bad Jokes

We were planning to tell a UDP joke on this slide...

## Networks: Classic Bad Jokes

![But we weren't sure if you would get it.](figures/eel.jpg){height=80%}

## Networks: CSC209 vs. CSC\*58

In this course, you learn what's necessary to _use_ TCP to communicate over a network. No UDP due to time constraints. If you want to learn more...

### [CSC358H5: Principles of Computer Networks](https://utm.calendar.utoronto.ca/course/csc358h5)

\small

Introduction to computer networks and systems programming of networks. Basic understanding of computer networks and network protocols. Network hardware and software, routing, addressing, congestion control, reliable data transfer, and socket programming.

### [CSC457H1: Principles of Computer Networks (formerly CSC358H1)](https://artsci.calendar.utoronto.ca/course/csc457h1)

\small

Fundamental principles of computer networks and currently used network architectures and protocols. [...] It will highlight the trade-offs (and approaches to navigate these trade-offs) in the design of computer network protocols. Theoretical but similar to CSC358H5.

### [CSC458/2209H1: Computer Networking Systems](https://artsci.calendar.utoronto.ca/course/csc458h1) [(similar to CSCD58H3)](https://utsc.calendar.utoronto.ca/course/cscd58h3)

\small

Computer networks with an emphasis on network systems, network programming, and applications. Networking basics: layering, routing, congestion control, and the global Internet. Network systems design and programming: Internet design, socket programming, and packet switching system fundamentals. Additional topics include network security, multimedia, software-defined networking, peer-to-peer networking, and online social networks.

## Networks: Internet Protocol (IP) Addresses

An _IP address_ identifies a specific host (computer) on a specific network.

- IPv4 addressing (most widespread) identifies hosts by four decimal 8-bit integers separated by dots, e.g., `128.100.3.30`. This is the current standard/default.
- IPv6 addressing identifies hosts by eight groups of four hex digits, separated by colons, e.g., `fe80:1234:0432:a2d8:61ff:fe8b:8924:c23f`.
- IPv4 is tractibly limited in the number of unique addresses, so IPv6 is the prospective replacement ([adoption is quite slow](https://www.google.com/intl/en/ipv6/statistics.html)).

You can use `dig` to find the IP address of a given URL. For example, Deerfield lab machines have IP address in the range `142.1.X.Y`.

```sh
$ dig dh2010pc11.utm.utoronto.ca | grep IN
;dh2010pc11.utm.utoronto.ca.	IN	A
dh2010pc11.utm.utoronto.ca. 8835 IN	A	142.1.46.80
```

## Networks: `localhost`

You may test your client and server programs by running both on the same machine.

- `127.0.0.1` is the "loopback" IP address, for when your program
  needs to communicate with another program on the same computer
- `localhost` is a _hostname_ "aliased" to `127.0.0.1`

![no place like `127.0.0.1`](figures/home.jpg){width=50%}

## Networks: TCP and UDP Ports

An IP address only identifies a host, but not the program running on the host.

- To communicate with a specific program on a host, you must
  specify a _port number_ between `0` and `65535`.
- For some of your homework, your client and server programs must use the same port number; otherwise, they cannot communicate.

### Port Number Conventions

- Ports in range `0-1023` are _well-known_ or _reserved_ (e.g., `22` for SSH, `80` for HTTP, `443` for HTTPS).
- Ports in range `1024-49151` are _registered_ (e.g., `3724` for World of Warcraft, `1313` for `hugo` servers).
- Ports in range `49152-65535` are _dynamic_
  - These are the ones you should typically pick, to avoid conflict.

[See IANA for list of port assignments](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.txt)

## Networks: Useful Commands Addresses & Ports

### `dig` - DNS lookup utility

\small

Performs DNS lookups and displays answers returned by the queried name server(s).

### `ifconfig` - show/manipulate routing, network devices, interfaces and tunnels

\small

The command `ifconfig` provides a rich source of information. They list the local (`l*`), cabled (`e*`) and wireless (`w*`) connections to your machine. Combine with other UNIX commands like `grep` to find what you're looking for, like IP addresses.

- If `ifconfig` doesn't exist, try `ip addr` (although output format is less pleasing).

### `ping` - send ICMP ECHO_REQUEST to network hosts

\small

To see if a server is online and reachable, use `ping` to send small packets to diagnose the connection. It'll show IP addresses of the server URLs. Use `ping6` or `ping -6` for IPv6.

```sh
ping math.toronto.edu # 128.100.68.3; what's coexeter?
ping utm.utoronto.ca  # 142.150.1.50; what's erin?
ping openai.com       # 13.107.238.51; why does it take so long?
ping google.com       # 142.251.41.78; why doesn't this take long?
ping6 google.com      # try ping -6 if ping6 isn't found
```

You might see some unexpected/interesting/creative results...

## Endianness: Byte Ordering

A byte (unit of 8 bits) is internally atomic (its own bits are consistently ordered). But how a sequence of bytes is ordered/interpreted depends.

- _Little-endian_ orders bytes from least to most significant. (Intel)
- _Big-endian_ orders bytes from most to least significant. (most others)

By convention, data being sent over a network must first be converted into _big endian_ (also known as _network byte order_).

- Any data received is thus assumed to be in network byte order.
- The endianness of the host is referred to as _host byte order_.

### Example -- IEEE-754 Floating Point

Consider the approximation $\tau \approx 6.28318530718$.

- Divide by $2$ twice to get the mantissa $1.5707963705062866$.
- The (biased) exponent is encoded as $129 = 10000001_2$
- The mantissa is encoded $4788187 = 10010010000111111011011_2$.

Try writing this IEEE-754 approximation in little-endian order.

## Endianness: Assumption & Conversion

To ensure portability of code, we always convert _host byte order_ to _network byte order_ even if they're the same. This behaviour is abstracted by conversion functions.

```c
#include <arpa/inet.h>    // most systems use this
#include <netinet/in.h>   // some systems require this instead

uint32_t htonl(uint32_t hostlong);    // host to network long
uint16_t htons(uint16_t hostshort);   // host to network short
uint32_t ntohl(uint32_t netlong);     // network to host long
uint16_t ntohs(uint16_t netshort);    // network to host short
```

Even port numbers must be converted, but ASCII text does not; why?

### Deeper Reading: Beej's Guide to Network Programming (3.2)

\small [`beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html`](https://beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html)

## Networks: Summary

### Transport Protocols

- UDP sends a _datagram_ (bundle of bounded data) between machines without checking receipts. (connectionless)
- TCP communicates between processes with _sockets_ using `write`/`read`. Receipt is implicitly guaranteed. (connection-oriented)

### IP Addresses & Ports

- IP addresses identify hosts on a network; standards are IPv4 and IPv6.
- `127.0.0.1`/`localhost` refers back to the host itself; useful for development.
- Applications communicate externally on ports of the host they're running from.

### Endianness

Network byte order is big-endian, and data is converted with system-dependent functions to ensure consistency and allow assumption.

### Deeper Reading: Beej's Guide to Network Programming (Section 3)

\small [`beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html`](https://beej.us/guide/bgnet/html/split/ip-addresses-structs-and-data-munging.html)
