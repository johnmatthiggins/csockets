#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define HTTP_HEADER_F_STR "HTTP/1.1 200 OK\r\n"\
                          "Content-Type: application/json\r\n"\
                          "Content-Length: %lu\r\n"\
                          "Server: C Echo Server\r\n"\
                          "\r\n"

int main(int argc, char* argv[]) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(4444),
        .sin_addr = inet_addr("127.0.0.1"),
    };

    int code = bind(
        socket_fd,
        (struct sockaddr*)&address,
        sizeof(address)
    );
    if (code == -1) {
        printf("ERROR!\n");
        return EXIT_FAILURE;
    }

    if (listen(socket_fd, 64) == -1) {
        printf("ERROR!\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in peer = (struct sockaddr_in){ 0 };
    socklen_t peer_len = sizeof(peer);

    while (1) {
        int connection_fd = accept(
            socket_fd,
            (struct sockaddr*)&peer,
            &peer_len
        );

        char request_buffer[0x1000] = { 0 };

        ssize_t request_length = recv(
            connection_fd,
            request_buffer,
            sizeof(request_buffer),
            MSG_PEEK
        );

        if (request_length > 0) {
            char* body_start = strstr(request_buffer, "\r\n\r\n") + 4;
            size_t header_length = (size_t)(body_start - request_buffer);
            size_t body_length = request_length - header_length;

            char response_buffer[0x1000]  = { 0 };
            snprintf(response_buffer, sizeof(response_buffer), HTTP_HEADER_F_STR, body_length);

            char* response_body_start = strstr(response_buffer, "\r\n\r\n") + 4;
            memcpy(response_body_start, body_start, body_length);

            size_t response_header_length = (size_t)(response_body_start - response_buffer);
            size_t response_length = response_header_length + body_length;

            response_buffer[response_length] = '\r';
            response_buffer[response_length + 1] = '\n';

            send(connection_fd, response_buffer, response_length, 0);
        }

        close(connection_fd);
    }

    return EXIT_SUCCESS;
}
