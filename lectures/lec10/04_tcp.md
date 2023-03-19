# The TCP/IP Model

## TCP/IP Model

![Layers in the TCP/IP Model](figures/tcpip.svg){width=95%}

## TCP/IP: Layers

The functionality of the TCP IP model is divided into four layers.

### Application Layer

In CSC209, when we define a message format, what we're really doing is defining an _application-layer protocol_, governing communication between server and client.

### Transport Layer

_Transport protocols_, such as TCP and UDP, govern how your OS "packages up" your application data to send it to another host over the network, and check to make sure that it arrived at the destination.

### Internet Layer

_Internet protocols_ such as IP, RIP, and OSPF govern how your data gets transferred from one Internet Service Provider (ISP; e.g., Bell, Rogers, UofT) to another.

### Link Layer

_Link-layer protocols_ deal with how your device physically transmits the data, e.g., wirelessly, or over a copper or fibre-optic cable.

## TCP/IP: Why Layers?

Suppose you're writing an application that uses local or remote connections. Imagine if you had to write different versions of code based on whether the client is connected,

- over a WiFi connection, LTE, or an Ethernet cable...
- to the local area network (LAN), a wide area network (WAN), or beyond...

The layers of the TCP/IP model separate different code to perform discrete networking functions. Learn more in CSC\*58!

### Further Reading: TCP/IP Stack from GURU99

[`www.guru99.com/tcp-ip-model.html#2`](https://www.guru99.com/tcp-ip-model.html#2)

## TCP/IP: Additional Features

TCP has many more features that are beyond the scope of our discussions for this course. Here are a couple of solutions to data loss issues...

- Flow control: If a computer is sending data too fast for the receiver to handle, TCP will automatically slow down to avoid data loss.
- Congestion control: If the network is too congested, TCP will automatically slow down to avoid data loss.

Networking theory is quite statistical; that's why CSC\*58 include an introductory probability/statistics course as their prerequisites.

### Common TCP/IP-modelled protocols

In addition to the literal marriage of TCP and IP, other protocols modelled this way include DNS, FTP, HTTP/S, SMTP, SNMP, TELNET.
