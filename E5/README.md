# Exercise 5
Implementation of a distributed system in which the client is provided a shell to access and execute files at the server's location.

The server, upon receiving a line of input from the client through the connected socket, forks a child process to find the file. The child forks another process which executes and redirects all input into a tmp[%d] file (where %d is the first child's PID) before terminating. The first child then reads the file's contents and feeds it back into the socket to the client, followed by a response indicating the end of the redirected stdout and the termination code of the child process.
