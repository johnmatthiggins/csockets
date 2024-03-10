#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>

#include "main.h"

int connection_fd;

/*
 * A simple web server in C.
 */
int main() {
    signal(SIGINT, handle_interrupt);

    errno = 0;
    int socket_fd= socket(AF_INET, SOCK_STREAM, 0);

    if (errno != 0) {
        print_socket_err(errno);
        return 1;
    }
    /* bind socket to 127.0.0.1:4080 */
    struct sockaddr_in address = {
        // IPv4 Address Family
        .sin_family = AF_INET,
        // Convert to Network Byte order (Big Endian)
        .sin_port = htons(PORT),
        // 0.0.0.0
        .sin_addr = inet_addr("0.0.0.0"),
    };

    errno = 0;
    int result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
    if (errno == EADDRINUSE) {
        address.sin_port = htons(PORT + 1);
        errno = 0;
        result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
    }

    if (errno != 0) {
        print_bind_err(errno);
        return 1;
    }

    errno = 0;
    if (listen(socket_fd, 50) == -1) {
        print_listen_err(errno);
    }

    struct sockaddr_in peer_address = (struct sockaddr_in){ 0 };
    socklen_t peer_address_size = sizeof(peer_address);

    printf("Waiting for connection at http://localhost:%d\n", PORT);

    int connection_fd;
    char buffer[BUFFER_LEN] = { 0 };

    for (;;) {
        connection_fd = accept(socket_fd, (struct sockaddr*)&peer_address, &peer_address_size);
        printf("Connection received!\n");
        errno = 0;
        printf("Checking with recvfrom...\n");
        printf("buffer size = %lu\n", sizeof(buffer));
        ssize_t length = recvfrom(connection_fd, buffer, sizeof(buffer), MSG_PEEK, NULL, NULL);

        if (length > 0) {
            printf("****** REQUEST ******\n%s\n", buffer);
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), HTTP_RESPONSE_F_STR, sizeof(HTML_BODY), HTML_BODY);

            send(connection_fd, buffer, strlen(buffer), 0);
            printf("****** RESPONSE ******\n%s\n", buffer);
            memset(buffer, 0, sizeof(buffer));

            close(connection_fd);
        }
    }

    printf("Closing socket...\n");
    close(socket_fd);

    return 0;
}

void print_socket_err(int err) {
    printf("ERROR: ");
    switch (err) {
        case EACCES:
            printf("Permission to create a socket of the specified type and/or protocol is denied.\n");
            break;
        case EAFNOSUPPORT:
            printf("The implementation does not support the specified address family.\n");
            break;
        case EINVAL:
            printf("Unknown protocol, or protocol family not available.\n");
            break;
        case EMFILE:
            printf("The per-process limit on the number of open files has been reached.\n");
            break;
        case ENFILE:
            printf("The system-wide limit on the total number of open files has been reached.\n");
            break;
        case ENOBUFS:
        case ENOMEM:
            printf("Insufficient memory is available. The socket cannot be created until sufficient resources are freed.\n");
            break;
        case EPROTONOSUPPORT:
            printf("The protocol type or the specified protocol is not supported within this domain.\n");
            break;
        default:
            break;
    }
}

void print_bind_err(int err) {
    switch (errno) {
        case EROFS:
            printf("EROFS: The socket inode would resied on a read-only filesystem.\n");
            break;
        case ENOMEM:
            printf("ENOMEM: Insufficient kernel memory was available.\n");
            break;
        case ENOENT:
            printf("ENOENT: A component in the directory prefix of the socket pathname does not exist.\n");
            break;
        case ENOTDIR:
            printf("ENOTDIR: A component of the path prefix is not a directory.\n");
            break;
        case ENAMETOOLONG:
            printf("ENAMETOOLONG: 'addr' is too long.\n");
            break;
        case ELOOP:
            printf("ELOOP: Too many symbolic links were encountered while resolving 'addr'.\n");
            break;
        case EFAULT:
            printf("EFAULT: 'addr' points outside the user's accessible address space.\n");
            break;
        case EADDRNOTAVAIL:
            printf("EADDRNOTAVAIL: A nonexistent interface was requested or the requested address was not local.\n");
            break;
        case EACCES:
            printf("EACCES: Search permission is denied on a component of the path prefix.\n");
            break;
        case ENOTSOCK:
            printf("ENOTSOCK: The file descriptor sockfd does not refer to a socket.\n");
            break;
        case EINVAL:
            printf("EINVAL: addrlen is wrong, or addr is not a valid address for this socket's domain.\n");
            break;
        case EBADF:
            printf("EBADF: sockfd is not a valid file descriptor.\n");
            break;
        case EADDRINUSE:
            printf("EADDRINUSE: The given address is already in use.\n");
            break;
        default:
            break;
    }
}

void print_listen_err(int err) {
    switch (errno) {
        case EADDRINUSE:
            printf("Another socket is already listening on the same port.\n");
            break;
        case EBADF:
            printf("The argument sockfd is not a valid file descriptor.\n");
            break;
        case ENOTSOCK:
            printf("The file descriptor sockfd does not refer to a socket.\n");
            break;
        case EOPNOTSUPP:
            printf("The socket is not of a type that supports the listen() operation.\n");
            break;
        default:
            break;
    }
}

void print_recv_err(int err) {
    switch (err) {
        case EAGAIN:
            printf("The socket is marked non-blocking, and the receive operation would block, or a receive timeout had been set, and the timeout expired before data were received.\n");
            break;
        case EBADF:
            printf("The argument socket is an invalid descriptor.\n");
            break;
        case ECONNRESET:
            printf("The connection is closed by the peer during the receive attempt on a socket.\n");
            break;
        case EFAULT:
            printf("The receive buffer pointer(s) point outside the process's address space.\n");
            break;
        case EINTR:
            printf("The receive was interrupted by delivery of a signal before any data were available.\n");
            break;
        case EINVAL:
            printf("MSG_OOB is set, but no out-of-band data is available.\n");
            break;
        case ENOBUFS:
            printf("An attempt to allocate memory buffer fails.\n");
            break;
        case ENOTCONN:
            printf("The socket is associated with a connection oriented protocol and has not been connected.\n");
            break;
        case ENOTSOCK:
            printf("The argument socket does not refer to a socket.\n");
            break;
        case EOPNOTSUPP:
            printf("The type and/or protocol of socket do not support option(s) specified in flags.\n");
            break;
        case ETIMEDOUT:
            printf("The connection timed out.\n");
            break;
        case EMSGSIZE:
            printf("The msg_iovlen member of the msghdr structure pointed to by messages is less than or equal to 0, or is greater than IOV_MAX.\n");
            break;
        case ENOMEM:
            printf("Insufficient memory is available.\n");
        default:
            break;
    }
}

void handle_interrupt(int sig) {
    printf("Ctrl-C detected...\n");
    printf("Closing sockets...\n");
    close(connection_fd);
    _exit(1);
}
