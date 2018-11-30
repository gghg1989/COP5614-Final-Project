# COP5614-Final-Project

## Structure
* lkm/				Network status scanner loadable kernel module
	* lkm/lkm_process_info.c 	lkm program to scan network status
	* lkm/Makefile				makefile for lkm
* print/			The program to save network status to file
	* print/bin/			Folder for print program excution files
	* print/lkm_print.c 	Print program source code
	* print/Makefile		makefile for print program
	* print/start_print.sh batch for autorun
* visualization/	Network topology visualization program
	* visualization/static		Static files folder for nodejs site
* README.md	Introduction for project

## Network status scanner loadable kernel module

This mod is use netfiler technique to catch the package used between virtual machines contact. To compile and install mod you need to excute the following sentance.


sudo make
sudo insmod lkm_process_info.ko

Then compile and run the user space program to get the information from the mod which is running in the kernal space.

gcc test.c -o test
./test

All the information collected by the mod are sent to this user program and stored in the "file.txt".

