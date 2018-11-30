# COP5614-Final-Project

## Structure
* lkm/				Network status scanner loadable kernel module
** 
* print/			The program to save network status to file
** 
* src/main.c office-hour-program source code
* src/util.h head file for utilities
* src/util.c utilities source code
* bin/	excution file for office-hour-program
* bin/main program entry point
* visualization/	Network topology visualization program
** 
* Makefile	makefile for office-hour-program
* README.md	introduction for project

## Network status scanner loadable kernel module

This mod is use netfiler technique to catch the package used between virtual machines contact. To compile and install mod you need to excute the following sentance.


sudo make
sudo insmod lkm_process_info.ko

Then compile and run the user space program to get the information from the mod which is running in the kernal space.

gcc test.c -o test
./test

All the information collected by the mod are sent to this user program and stored in the "file.txt".

