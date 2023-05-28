#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int S_fd, C_fd, file_fd;
    int S_len, C_len;
    struct sockaddr_in S_address;
    struct sockaddr_in C_address;
    int result;
    char buff[BUFSIZ];

    // Create a socket
    S_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (S_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Open or create a file for writing
    file_fd = open("server_file.txt", O_RDWR | O_APPEND | O_CREAT, 0777);
    if (file_fd == -1) {
        perror("open");
        exit(1);
    }

    // Set up server address
    S_address.sin_family = AF_INET;
    S_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    S_address.sin_port = htons(9224);
    S_len = sizeof(S_address);

    // Bind the socket to the server address
    result = bind(S_fd, (struct sockaddr *) &S_address, S_len);
    if (result == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    result = listen(S_fd, 5);
    if (result == -1) {
        perror("listen");
        exit(1);
    }

    printf("Server process: %d, waiting.\n", getpid());

    // Accept a client connection
    C_len = sizeof(C_address);
    C_fd = accept(S_fd, (struct sockaddr *) &C_address, &C_len);
    if (C_fd == -1) {
        perror("accept");
        exit(1);
    }

    ssize_t bytesRead;
    // Read data from the client and write it to the file
    while ((bytesRead = read(C_fd, buff, BUFSIZ)) > 0) {
        ssize_t bytesWritten = write(file_fd, buff, bytesRead);
        if (bytesWritten == -1) {
            perror("write");
            exit(1);
        }
    }

    if (bytesRead == -1) {
        perror("read");
        exit(1);
    }

    printf("Server read and wrote the file successfully.\n");

    // Close the connections and file
    close(C_fd);
    close(S_fd);

    return 0;
}
