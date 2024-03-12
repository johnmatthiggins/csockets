#ifndef _MAIN_H
#define _MAIN_H

#define HTTP_RESPONSE_F_STR "HTTP/1.1 200 OK\r\n"\
                      "Content-Type: text/html\r\n"\
                      "Content-Length: %lu\r\n"\
                      "Server: C Echo Server\r\n"\
                      "\r\n%s\r\n"

// Use one megabyte buffer to read in requests...
#define ONE_MiB (0x1 << 20)
#define PORT 4444

/* prints error codes for socket() function. */
void print_socket_err(int err);

/* prints error codes for bind() function. */
void print_bind_err(int err);

/* prints error codes for listen() function. */
void print_listen_err(int err);

/* prints error codes for recv() function. */
void print_recv_err(int err);

/* reads body of http request into buffer */
size_t read_body(char* http_request, size_t request_len, char* buffer, size_t buffer_len);

/* Handles when user hits CTRL-C */
void handle_interrupt(int sig);

#endif
