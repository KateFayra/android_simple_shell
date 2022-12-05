# Simple Shell by Nikki Fayra
A simple shell for Android using the SDK (Java Standard Development Kit) and NDK (Native C Development Kit) created for CS241 systems programming Android examples while I was a course assistant.

Can be installed on any Android device as an APK and can execute any standard UNIX system utilities such as: ls, cat, uname, and ps. 

It uses fork() and exec() in the native C code to do so, while writing output and receiving user input from the Java code.

It also supports the standard "cd" command to change the current working directory. 

The shell prompt emulates Bash by showing the username, hostname, and current working directory.

###### Note
It can not currently handle programs which need additional input from stdin after initial execution as it does not keep a pipe open between the code and the child process.
