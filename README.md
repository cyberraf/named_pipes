# named_pipes


# Description:
	This program uses named pipes in which one process sends a string message to a second process,
	and the second process reverses the case of each character in the message and sends it back
	to the first process. For example, if the first process sends the message "My Name is Cyberraf", the 
	second process will return "mY nAME IS cYBERRAF". This will require using two pipes, one for sending 
	the original message from the first to the second process and the other for sending the
	modified message from the second to the first process. You can write this program using
	either UNIX or Windows pipes.

# To Compile: 

gcc -Wall named_pipes.c -o named_pipes


![alt text](https://github.com/cyberraf/named_pipes/blob/main/compilation.png)

# To Execute: 

./named_pipes

![alt text](https://github.com/cyberraf/named_pipes/blob/main/execution.png)
