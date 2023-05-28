#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int sock_fd, len, file_fd;
    struct sockaddr_in address;
    int result;
    char buff[BUFSIZ];

    // Create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Open the file for reading
    file_fd = open(argv[1], O_RDONLY);
    if (file_fd == -1) {
        perror("open");
        exit(1);
    }

    // Set up server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9224);
    len = sizeof(address);

    // Connect to the server
    result = connect(sock_fd, (struct sockaddr *)&address, len);
    if (result == -1) {
        perror("connect");
        exit(1);
    }

    ssize_t bytesRead;
    // Read data from the file and write it to the server
    while ((bytesRead = read(file_fd, buff, BUFSIZ)) > 0) {
        ssize_t bytesWritten = write(sock_fd, buff, bytesRead);
        if (bytesWritten == -1) {
            perror("write");
            exit(1);
        }
    }

    if (bytesRead == -1) {
        perror("read");
        exit(1);
    }

    printf("Client wrote the file successfully.\n");

    // Close the socket and file
    close(sock_fd);
    close(file_fd);

    return 0;
}
