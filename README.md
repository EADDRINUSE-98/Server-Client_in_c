# Server-Client_in_c
This is a basic server-client program in c that uses TCP protocol (or UDP protocol in future ... maybe) for establishing connection between server and client. This small project is indented for educational purposes and should not be used for actual communication over the internet.
___
# Purpose
Since most of the programming languages are written in c, like python, java, perl and so on, I decided to try some socket programming in c. Purpose of this straightforward projects is to understand what is actually happening under the hood. For e.g., when we make a socket in python using `socket()` function, python only takes two arguments but in c `socket()` function take three arguments.
___
# Usage
This usage section it particularly aiming towards users who are maniac like me and compile their c programs on terminal. Anyone who is using any type of IDE like vscode, etc, can skip the `tcpserver.c` usage section but must pay attention to `tcpclient.c` usage section.
## Running `tcpserver.c`
To run `tcpserver.c`, we must first compile it with our beloved `gcc` compiler by running following command:
```
gcc tcpserver.c -o tcpserver
```

Now execute the executable binary named `tcpserver` with following command:
```
./tcpserver
```

> [!Warning] : To execute `tcpserver` with above mentioned command correctly, the executable file must be in the current working directory.

## Running `tcpclient.c`
___
# Limitations and Considerations
