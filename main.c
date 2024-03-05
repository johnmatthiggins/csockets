#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

/*
 * A simple web server in C.
 */
int main() {
    errno = 0;
    int32_t fd = socket(AF_INET, SOCK_STREAM, 0);

    uint32_t exit_code = 0;
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
    } else {
    }

    return exit_code;
}
