#define HTTP_RESPONSE "HTTP/1.1 200 OK\n"\
                      "Content-Type: application/json\n"\
                      "Server: Little C Server\n\n"\
                      "{ message: \"Hello world\"!}\n"

void print_socket_err(int err);
void print_bind_err(int err);
void print_listen_err(int err);
void print_recv_err(int err);
