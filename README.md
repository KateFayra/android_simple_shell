# android_simple_shell
A simple shell for Android using the SDK and NDK.
This is a simple shell program for Android using the SDK and NDK.

It can execute any standard UNIX system utilities such as: ls, cat, uname, and ps. It uses fork() and exec() in the native C code to do so, while writing output and receiving user input from the Java code.

It also supports the standard "cd" command to change the current working directory. The shell prompt emulates Bash by showing the username, hostname, and current working directory.

######Note
It can not currently handle programs which need additional input from stdin after initial execution as it does not keep a pipe open between the code and the child process.
