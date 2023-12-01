# ft_irc

# 👀 Project overview

The ft_irc project is a C++ based implementation of an IRC (Internet Relay Chat) server, adhering to the IRC protocol for real-time text messaging. This server allows users to connect, join channels, and communicate privately or in groups.

# 💡 Epoll: Key Learning and Implementation
This project highlights the use of epoll, a Linux I/O event notification facility, for efficient multi-socket monitoring.

***Epoll Explained***: It's a high-performance tool in Linux for monitoring numerous file descriptors, crucial for scalable network servers.
Usage in Project: epoll is employed to oversee client sockets, providing efficient handling of multiple connections.
Setup and Execution

Clone the repository 
```
make
./ircserv <port> <password>
```

- `port`: The port number for the IRC server.
- `password`: The server connection password.

# 💻 Testing and Compatibility
- ***Reference Client***: `Irssi`, a command-line based IRC client, known for its reliability and widespread usage.
- ***Additional Testing***: Extensively tested with `netcat` (nc), ensuring compatibility and robustness.
- ***Testing Scenarios***: Stress testing network capabilities, handling client disconnections, and efficient broadcasting.
- ***Compatibility***: Developed and tested on various Linux distributions supporting epoll.

# Ressources
- [Horse Doc - Modern IRC Documentation](https://modern.ircdocs.horse)
- [The method to epoll's madness](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)

# 👥 Contribution and Development
This project is a collaborative effort :

<p align="center">
<a href="http://github.com/lmelard" alt="lmelard github profile"><img src="https://github.com/lmelard.png" width="60px style="border-radius:50%"/></a>
<a href="http://github.com/tiny-chris" alt="tiny-chris github profile"><img src="https://github.com/tiny-chris.png" width="60px style="border-radius:50%"/></a>
</p>



