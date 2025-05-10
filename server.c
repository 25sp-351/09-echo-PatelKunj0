#include "echo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <pthread.h>

typedef struct {
    int client_fd;
    int verbose;
} thread_args_t;

static void *thread_func(void *arg) {
    thread_args_t *args = arg;
    handle_connection(args->client_fd, args->verbose);
    close(args->client_fd);
    free(args);

    return NULL;
}

int run_server(int port, int verbose) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");

        return EXIT_FAILURE;
    }

    int optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    struct sockaddr_in srv_addr = {
        .sin_family      = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port        = htons(port)
    };

    if (bind(server_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
        perror("bind");
        close(server_fd);

        return EXIT_FAILURE;
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        
        return EXIT_FAILURE;
    }

    printf("Echo server (multi-threaded) listening on port %d %s\n",
           port, verbose ? "(verbose)" : "");

    fflush(stdout);

    for (;;) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        if (verbose) {
            printf("New connection accepted\n");
            fflush(stdout);
        }

        thread_args_t *args = malloc(sizeof(*args));

        if (!args) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        args->client_fd = client_fd;
        args->verbose   = verbose;

        pthread_t tid;

        if (pthread_create(&tid, NULL, thread_func, args) != 0) {
            perror("pthread_create");
            close(client_fd);
            free(args);
        } 
        else {
            pthread_detach(tid);
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
