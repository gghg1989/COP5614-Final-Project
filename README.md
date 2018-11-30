# COP5614-Final-Project

The final project for COP 5614 Operating Systems - Fall 2018.
* Team #12:
	* Liqun Yang 		PID: 6072714
	* Yuzhou Feng		PID: 4884556
	* Tianren Yang		PID: 5966794

## Structure
* lkm/				Network status monitoring loadable kernel module
	* lkm/lkm_process_info.c 	lkm program to monitoring network status
	* lkm/Makefile				makefile for lkm
* print/			The program to save network status to file
	* print/bin/			Folder for print program excution files
	* print/lkm_print.c 	Print program source code
	* print/Makefile		makefile for print program
	* print/start_print.sh batch for autorun
* visualization/	Network topology visualization program
	* visualization/static		Static files folder for nodejs site
		* visualization/static/main.js		Front end code
		* visualization/static/com.png		Computer icon for visualization
		* visualization/static/data.json	Generated data for network topology
	* visualization/index.html 		Web page for network topology visualization
	* visualization/package.json 	Nodejs project configuration file
	* visualization/server.js 		Nodejs project serving code
* README.md	Introduction for project

## Docker Environment Setup

## Network Status Monitoring Loadable Kernel Module

This Loadable Kernel Module program is basing on netfiler framework, for which monitoring the communications between docker containers. 

### Usage
* To compile and install mod you need to excute the following sentance.
```bash
sudo make
sudo insmod lkm_process_info.ko
```
* Then compile and run the user space program to get the information from the mod which is running in the kernal space.
	* Get into print folder
	```bash
	cd print/
	```
	* Compile the print program by Makefile
	```bash
	make
	```

All the information collected by the mod are sent to this user program and stored in the "file.txt".

## Network Topology Visualization

## Docker Engine Integration

## Contribution
* Liqun Yang(6072714)
	* Part 2: Network Monitoring Tool Design with LKM and C
* Yuzhou Feng(4884556)
	* Part 1: Docker Environment Setup
	* Part 3: Network Topology Visualization with Nodejs and Javascript
	* Documentation
	* Video
* Tianren Yang(5966794)
	* Part 4: Docker Engine Integration