# File Transfer - Client-Server

This repository contains a client-server implementation for transferring files over a TCP/IP network. The client program reads a file from the local system and sends it to the server, which writes the received data into a file on its system.

## Prerequisites
To run these programs, you need a Linux system with a C compiler and the necessary development libraries. Make sure you have the following libraries installed:

* libc-dev
* libpthread-dev

## Instructions
* Clone this repository to your local machine:
***git clone https://github.com/your-username/repo-name.git***

* Compile the server program using the following command:

***gcc server.c -o server***

* Compile the client program using the following command:

***gcc client.c -o client***

* Run the server program in one terminal window:

***./server***

* In another terminal window, run the client program with the file you want to transfer as a command-line argument:

***./client /path/to/file***

The client program will establish a connection with the server and send the file. The server will write the received data into a file named server_file.txt in the current directory.

Once the transfer is complete, both the client and server will display a success message.
