# Server-Client_in_c
This is a basic server-client program in c that uses TCP protocol (or UDP protocol in future ... maybe) for establishing connection between server and client. This small project is indented for educational purposes and should not be used for actual communication over the internet.
___
# Purpose
Since most of the programming languages are written in c, like python, java, perl and so on, I decided to try some socket programming in c. Purpose of this straightforward projects is to understand what is actually happening under the hood. For e.g., when we make a socket in python using `socket()` function, python only takes two arguments but in c `socket()` function take three arguments.
___
# Usage
This usage section it particularly aiming towards users who are maniac like me and compile their c programs on terminal manually. Anyone who is using any type of IDE like vscode, etc, can skip the `tcpserver.c` usage section but must pay attention to `tcpclient.c` usage section.
## Running `tcpserver.c`
To run `tcpserver.c`, we must first compile it with our beloved `gcc` compiler by running following command:
```
gcc tcpserver.c -o tcpserver
```

Now execute the executable binary named `tcpserver` with following command:
```
./tcpserver
```

> **Warning:** To execute `tcpserver` with above mentioned command correctly, the executable file must be in the current working directory.

## Running `tcpclient.c`
Work in progress =)
___
# Limitations and Considerations
- This work is only for educational purpose and nobody is encouraged to use this work for any confidential communication since there is no network encryption mechanism implemented.
- This code base was written in a linux environment. Therefore, some c libraries used in this project can only be accessed in unix-like system environment. For e.g. `unistd.h`, `sys/socket.h`, `sys/types.h`, `signal.h`, etc, can properly used in unix-like system envirnoment.
- Since this project is for just goofing around, anybody can clone it and fork it as per their needs.
- If any user who wants to understand what each of the functions and libraries do, they can either use `man` command to view the manual pages of each function, or use [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/) for better understanding of socket programming in c language.