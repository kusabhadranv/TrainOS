TOS,Train Operating System, is an operating system for intel x86 based PCs. It is a small pre-emptive multi-tasking operating system that can boot on any intel based PCs. Key features of TOS are: 
* Replicates the functionality of the following Linux commands:
  * help
  * clear
  * shell
  * pong
  * echo <msg>
  * ps
  * history
  * !<number>
  * about
* Controls an external model train via a serial line interface. The train application is created as seperate process with in the OS that handles 4 different configurations of train starting points with and without zamboni. The train is programmed to avoid the zamboni to reach the wagon, dock it and get it back to the starting point. 
  
  Visual details on the documentation pdf.
