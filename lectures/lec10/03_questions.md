# Trivia Questions

## Messages: Questions

1. How do we know when we have received a complete (not partial) message?
2. Are there alternative techniques for determining that we have received a complete message?

## Messages: Questions & Answers

1. How do we know when we have received a complete (not partial) message?

   - _We define a byte sequence that indicates the end of a message. In text-based protocols, the most common convention is to signify an end-of-message with a CRLF (carriage-return + newline, or `\r\n`) sequence. Actual message content must not contain any instances of this sequence._

2. Are there alternative techniques for determining that we have received a complete message? _Yes. Two common techniques:_

   - _Define a fixed-length message format (i.e., every message must be identical in length). Then check the number of bytes received in `read`/`recv`._
   - _Define a fixed-length "header" that contains an integer representing the length of the remainder of the message. Also check the number of bytes received._

3. Follow-up: in the header solution, would you still need any length standardization?

   - _Yes, but much less! You need only standardize the size of the header, which is much smaller than the rest of the message (which can now be of variable length indicated in the header)._

## Sending/Receiving: Questions

3. Is it possible for the server (or client) to call `read()` on a socket and receive more than one message? Hint: how would pipes work?
4. What happens if your PC receives data from the network, but your program isn't ready to call `read()`/`recv()` yet, because it is busy doing something else.

## Sending/Receiving: Questions & Answers

3. Is it possible for the server (or client) to call `read()` on a socket and receive more than one message? Hint: how would pipes work?

   - _Yes. This might happen if the sender is sending the messages faster than you are reading them. In this case, you must save the messages in a **buffer** and handle them one at a time._

4. What happens if your PC receives data from the network, but your program isn't ready to call `read()`/`recv()` yet, because it is busy doing something else.

   - _The OS saves it in a buffer, until your program calls `read()`/`recv()`._
