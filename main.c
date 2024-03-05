#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>

/*
 * A simple web server in C.
 */
int main() {
    errno = 0;
    int32_t sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (errno != 0) {
        printf("Socket could not be opened!\n");
        switch (errno) {
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
        return 1;
    }

    /* bind socket to 127.0.0.1:4080 */
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = 4080,
        .sin_addr = INADDR_ANY,
    };

    errno = 0;
    uint32_t result = bind(sfd, (struct sockaddr *)&address, sizeof(address));

    if (result == -1) {
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
        return 1;
    }

    errno = 0;
    if (listen(sfd, 50) == -1) {
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

    struct sockaddr_in peer_address = (struct sockaddr_in){ 0 };
    printf("Waiting for connection...\n");
    socklen_t peer_address_size = sizeof(peer_address);
    int32_t cfd = accept(sfd, (struct sockaddr*)&peer_address, &peer_address_size);

    close(sfd);

    return 0;
}
