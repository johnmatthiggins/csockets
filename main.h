#ifndef _MAIN_H
#define _MAIN_H

#define HTTP_RESPONSE_F_STR "HTTP/1.0 200 OK\r\n"\
                      "Content-Type: text/html\r\n"\
                      "Content-Length: %lu\r\n"\
                      "Server: Little C Server\r\n"\
                      "\r\n%s\r\n"

#define HTML_BODY "<!doctype html>\r\n"\
                  "<html lang=\"en\">\r\n"\
                  "  <head><meta charset=\"UTF-8\"></head>\r\n"\
                  "  <body>\r\n"\
                  "    <h1>Hello world!</h1>\r\n"\
                  "  </body>\r\n"\
                  "</html>\r\n"

#define BUFFER_LEN 1024
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
int read_body(char* http_request, char* buffer, size_t buffer_len);

/* Handles when user hits CTRL-C */
void handle_interrupt(int sig);

#endif
