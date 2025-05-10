#ifndef ECHO_H
#define ECHO_H

#include <unistd.h>

#define DEFAULT_PORT 2345
#define BACKLOG      1
#define BUF_SIZE     1024

void handle_connection(int client_fd, int verbose);
int run_server(int port, int verbose);

#endif
