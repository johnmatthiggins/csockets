#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\n"\
                      "Content-Type: application/json\r\n"\
                      "Content-Length: 30\r\n"\
                      "Server: Little C Server\r\n\r\n"\
                      "{ message: \"Hello world!\" }\r\n"

#define BUFFER_LEN 1024

/* prints error codes for socket() function. */
void print_socket_err(int err);

/* prints error codes for bind() function. */
void print_bind_err(int err);

/* prints error codes for listen() function. */
void print_listen_err(int err);

/* prints error codes for recv() function. */
void print_recv_err(int err);
