/*
 * This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International license.
 *
 * Author: D'Arcy Smith (ds@programming101.dev)
 *
 * You are free to:
 *   - Share: Copy and redistribute the material in any medium or format.
 *   - Under the following terms:
 *       - Attribution: You must give appropriate credit, provide a link to the license, and indicate if changes were made.
 *       - NonCommercial: You may not use the material for commercial purposes.
 *       - NoDerivatives: If you remix, transform, or build upon the material, you may not distribute the modified material.
 *
 * For more details, please refer to the full license text at:
 * https://creativecommons.org/licenses/by-nc-nd/4.0/
 */

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <p101_env/env.h>
#include <p101_posix/p101_unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void           setup_signal_handler(void);
static void           sigint_handler(int signum);
static void           parse_arguments(int argc, char *argv[], char **ip_address, char **port);
static void           handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, in_port_t *port);
static in_port_t      parse_in_port_t(const char *binary_name, const char *port_str);
_Noreturn static void usage(const char *program_name, int exit_code, const char *message);
static void           convert_address(const char *address, struct sockaddr_storage *addr);
static int            socket_create(int domain, int type, int protocol);
static void           socket_bind(int sockfd, struct sockaddr_storage *addr, in_port_t port);
static void           start_listening(int server_fd, int backlog);
static void           handle_connection(int client_sockfd, char *command);
static void           socket_close(int sockfd);

int tokenizeString(char *string, const char *delimiter, char *array[]);

#define UNKNOWN_OPTION_MESSAGE_LEN 24
#define BASE_TEN 10
#define LENGTH 512

static volatile sig_atomic_t exit_flag = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

int main(int argc, char *argv[])
{
    char                   *address;
    char                   *port_str;
    in_port_t               port;
    int                     sockfd;
    int                     enable;
    struct sockaddr_storage addr;

    /**
     * IO multiplexing variables
     */
    int   *client_sockets;
    size_t max_clients;
    int    new_socket;
    int    sd;
    fd_set readfds;

    max_clients    = 0;
    client_sockets = NULL;

    address  = NULL;
    port_str = NULL;
    parse_arguments(argc, argv, &address, &port_str);
    handle_arguments(argv[0], address, port_str, &port);
    convert_address(address, &addr);
    sockfd = socket_create(addr.ss_family, SOCK_STREAM, 0);
    enable = 1;

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1)
    {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    socket_bind(sockfd, &addr, port);
    start_listening(sockfd, SOMAXCONN);
    //    setup_signal_handler();

    /**
     * replace this?
     */
    while(!exit_flag)
    {
        int max_fd;
        int activity;

        setup_signal_handler();
        // Clear the socket set
#ifndef __clang_analyzer__
        FD_ZERO(&readfds);
#endif
        // Add the server socket to the set
        FD_SET((unsigned int)sockfd, &readfds);
        max_fd = sockfd;

        // Add the client sockets to the set
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];

            if(sd > 0)
            {
                FD_SET((unsigned int)sd, &readfds);
            }
            if(sd > max_fd)
            {
                max_fd = sd;
            }
        }

        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if(activity < 0)
        {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        // Handle new client connections
        if(FD_ISSET((unsigned int)sockfd, &readfds))
        {
            int                    *temp;
            struct sockaddr_storage client_addr;
            socklen_t               addrlen;

            addrlen = sizeof(addr);

            new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen);

            if(new_socket == -1)
            {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection established\n");

            // Increase the size of the client_sockets array
            max_clients++;
            temp = (int *)realloc(client_sockets, sizeof(int) * max_clients);

            if(temp == NULL)
            {
                perror("realloc");
                free(client_sockets);
                exit(EXIT_FAILURE);
            }
            else
            {
                client_sockets                  = temp;
                client_sockets[max_clients - 1] = new_socket;
            }
        }

        // Handle incoming data from existing clients
        /**
         * bytes_read = read(fd)
         * if bytes_read() > 0
         * execute command
         * if error
         * remove fd from list
         */
        for(size_t i = 0; i < max_clients; i++)
        {
            sd = client_sockets[i];
            if(FD_ISSET((unsigned int)sd, &readfds))
            {
                char     buffer[LENGTH];
                ssize_t  bytes_read;
                uint16_t message_size;

                memset(buffer, 0, LENGTH);

                read(sd, &message_size, sizeof(uint16_t));
                bytes_read = read(sd, buffer, message_size);
                printf("Buffer: %s\n", buffer);
                if(bytes_read == 0)
                {
                    // Client closed the connection
                    printf("Client %d disconnected\n", sd);
                    close(sd);
                    FD_CLR((unsigned int)sd, &readfds);
                    // Handle client cleanup, if necessary
                }
                else if(bytes_read < 0)
                {
                    // Error reading from client
                    perror("Read error");
                    close(sd);
                    FD_CLR((unsigned int)sd, &readfds);
                    // Handle error, if necessary
                }
                else
                {
                    // Handle incoming data from the client
                    buffer[bytes_read - 1] = '\0';    // Ensure null termination
                    handle_connection(sd, buffer);
                }
            }

            /**
             * testing up above
             */
            //            char     buffer[LENGTH];
            //            uint16_t message_size;
            //            ssize_t  bytes_read;
            //            sd = client_sockets[i];
            //            //            printf("Read_message_called\n");
            //
            //            memset(buffer, 0, LENGTH);
            //            //            memset(buffer, 0, LENGTH);
            //
            //            // These 2 are normal
            //            read(sd, &message_size, sizeof(uint16_t));
            //            bytes_read = read(sd, buffer, message_size);
            //
            //            if(bytes_read > 0)
            //            {
            //                printf("Bytes read: %d\n", (int)bytes_read);
            //                printf("Buffer: %s\n", buffer);
            //                buffer[bytes_read - 1] = '\0';
            //                handle_connection(sd, buffer);
            //                //
            //                //                FD_CLR((unsigned int)sd, &readfds);
            //                //                close(sd);
            //                //                // Remove the closed socket from the set
            //                //                client_sockets[i] = '\0';
            //                //                max_clients--;
            //            }
            //            else if(bytes_read < 0)
            //            {
            //                FD_CLR((unsigned int)sd, &readfds);
            //                close(sd);
            //                // Remove the closed socket from the set
            //                client_sockets[i] = '\0';
            //                max_clients--;
            //                //                handle_connection(sd, buffer);
            //            }
            //            else
            //            {
            //                continue;
            //            }

            /**
             * stuff that owrks
             */
            //            handle_connection(sd);
            //            printf("\nClient disconnected\n");
            //            close(sd);
            //            FD_CLR((unsigned int)sd, &readfds);
            //            // Remove the closed socket from the set
            //            client_sockets[i] = '\0';
            //            max_clients--;
        }
    }

    // Cleanup and close all client sockets
    for(size_t i = 0; i < max_clients; i++)
    {
        sd = client_sockets[i];

        if(sd > 0)
        {
            socket_close(sd);
        }
    }

    free(client_sockets);
    socket_close(sockfd);
    printf("Server exited successfully.\n");

    return EXIT_SUCCESS;
}

static void parse_arguments(int argc, char *argv[], char **ip_address, char **port)
{
    int opt;

    opterr = 0;

    while((opt = getopt(argc, argv, "h")) != -1)
    {
        switch(opt)
        {
            case 'h':
            {
                usage(argv[0], EXIT_SUCCESS, NULL);
            }
            case '?':
            {
                char message[UNKNOWN_OPTION_MESSAGE_LEN];

                snprintf(message, sizeof(message), "Unknown option '-%c'.", optopt);
                usage(argv[0], EXIT_FAILURE, message);
            }
            default:
            {
                usage(argv[0], EXIT_FAILURE, NULL);
            }
        }
    }

    if(optind >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The ip address and port are required");
    }

    if(optind + 1 >= argc)
    {
        usage(argv[0], EXIT_FAILURE, "The port is required");
    }

    if(optind < argc - 2)
    {
        usage(argv[0], EXIT_FAILURE, "Error: Too many arguments.");
    }

    *ip_address = argv[optind];
    *port       = argv[optind + 1];
}

static void handle_arguments(const char *binary_name, const char *ip_address, const char *port_str, in_port_t *port)
{
    if(ip_address == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The ip address is required.");
    }

    if(port_str == NULL)
    {
        usage(binary_name, EXIT_FAILURE, "The port is required.");
    }

    *port = parse_in_port_t(binary_name, port_str);
}

in_port_t parse_in_port_t(const char *binary_name, const char *str)
{
    char     *endptr;
    uintmax_t parsed_value;

    errno        = 0;
    parsed_value = strtoumax(str, &endptr, BASE_TEN);

    if(errno != 0)
    {
        perror("Error parsing in_port_t");
        exit(EXIT_FAILURE);
    }

    // Check if there are any non-numeric characters in the input string
    if(*endptr != '\0')
    {
        usage(binary_name, EXIT_FAILURE, "Invalid characters in input.");
    }

    // Check if the parsed value is within the valid range for in_port_t
    if(parsed_value > UINT16_MAX)
    {
        usage(binary_name, EXIT_FAILURE, "in_port_t value out of range.");
    }

    return (in_port_t)parsed_value;
}

_Noreturn static void usage(const char *program_name, int exit_code, const char *message)
{
    if(message)
    {
        fprintf(stderr, "%s\n", message);
    }

    fprintf(stderr, "Usage: %s [-h] <ip address> <port>\n", program_name);
    fputs("Options:\n", stderr);
    fputs("  -h  Display this help message\n", stderr);
    exit(exit_code);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void sigint_handler(int signum)
{
    exit_flag = 1;
}

#pragma GCC diagnostic pop

static void convert_address(const char *address, struct sockaddr_storage *addr)
{
    memset(addr, 0, sizeof(*addr));

    if(inet_pton(AF_INET, address, &(((struct sockaddr_in *)addr)->sin_addr)) == 1)
    {
        addr->ss_family = AF_INET;
    }
    else if(inet_pton(AF_INET6, address, &(((struct sockaddr_in6 *)addr)->sin6_addr)) == 1)
    {
        addr->ss_family = AF_INET6;
    }
    else
    {
        fprintf(stderr, "%s is not an IPv4 or an IPv6 address\n", address);
        exit(EXIT_FAILURE);
    }
}

static int socket_create(int domain, int type, int protocol)
{
    int sockfd;

    sockfd = socket(domain, type, protocol);

    if(sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

static void socket_bind(int sockfd, struct sockaddr_storage *addr, in_port_t port)
{
    char      addr_str[INET6_ADDRSTRLEN];
    socklen_t addr_len;
    void     *vaddr;
    in_port_t net_port;

    net_port = htons(port);

    if(addr->ss_family == AF_INET)
    {
        struct sockaddr_in *ipv4_addr;

        ipv4_addr           = (struct sockaddr_in *)addr;
        addr_len            = sizeof(*ipv4_addr);
        ipv4_addr->sin_port = net_port;
        vaddr               = (void *)&(((struct sockaddr_in *)addr)->sin_addr);
    }
    else if(addr->ss_family == AF_INET6)
    {
        struct sockaddr_in6 *ipv6_addr;

        ipv6_addr            = (struct sockaddr_in6 *)addr;
        addr_len             = sizeof(*ipv6_addr);
        ipv6_addr->sin6_port = net_port;
        vaddr                = (void *)&(((struct sockaddr_in6 *)addr)->sin6_addr);
    }
    else
    {
        fprintf(stderr, "Internal error: addr->ss_family must be AF_INET or AF_INET6, was: %d\n", addr->ss_family);
        exit(EXIT_FAILURE);
    }

    if(inet_ntop(addr->ss_family, vaddr, addr_str, sizeof(addr_str)) == NULL)
    {
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }

    printf("Binding to: %s:%u\n", addr_str, port);

    if(bind(sockfd, (struct sockaddr *)addr, addr_len) == -1)
    {
        perror("Binding failed");
        fprintf(stderr, "Error code: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printf("Bound to socket: %s:%u\n", addr_str, port);
}

static void start_listening(int server_fd, int backlog)
{
    if(listen(server_fd, backlog) == -1)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Listening for incoming connections...\n");
}

static void setup_signal_handler(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = sigint_handler;
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static void handle_connection(int client_sockfd, char *command)
{
    pid_t childPid;

    /**
     * Testing p101_dup2 here
     */

    //    ssize_t  message_len;
    //    char     buffer[UINT16_MAX];
    //    uint16_t size;
    //
    //    message_len = read(client_sockfd, &size, sizeof(uint16_t));    // Write the size of the command
    //    read(client_sockfd, buffer, message_len);

    childPid = fork();
    if(childPid == 0)
    {
        struct p101_error *error;
        struct p101_env   *env;
        //        int temp_fd = open("temp_file.txt", O_WRONLY | O_CREAT, 0666);
        //        dup2(client_sockfd, STDOUT_FILENO);

        //        printf("Child starts\n");
        char *path = getenv("PATH");

        const char strTokValue[2] = ":";
        const char argTokValue[2] = " ";
        char      *argList[LENGTH];
        char      *listOfPaths[LENGTH];

        //    printf("Total path: %s\n", path);

        int arraySize = tokenizeString(path, strTokValue, (char **)&listOfPaths);

        error = p101_error_create(false);
        env   = p101_env_create(error, true, NULL);

        //        uint16_t size;
        //        char     buffer[UINT16_MAX + 1];

        //        read(client_sockfd, &size, sizeof(uint16_t));
        //        // These 2 are normal
        //        read(client_sockfd, buffer, size);
        //        printf("Buffer: %s\n", buffer);
        tokenizeString(command, argTokValue, (char **)&argList);
        if(argList[0] != NULL)
        {
            char currentPath[LENGTH];

            for(int i = 0; i < arraySize; i++)
            {
                strncpy(currentPath, listOfPaths[i], sizeof(currentPath) - 1);
                strncat(currentPath, "/", sizeof(currentPath) - strlen(currentPath) - 1);
                strncat(currentPath, argList[0], sizeof(currentPath) - strlen(currentPath) - 1);

                if(access(currentPath, X_OK) != -1)
                {
                    //                printf("Current Path: %s\n", currentPath);
                    break;
                }

                if(i == arraySize - 1)
                {
                    perror("Invalid path\n");
                    exit(EXIT_FAILURE);
                }
            }

            if(p101_dup2(env, error, client_sockfd, STDOUT_FILENO) == -1)
            {
                perror("Dup2 Failed");
                exit(EXIT_FAILURE);
            }

            free(error);
            free(env);

            if(execv(currentPath, argList) == -1)
            {
                //                if(errno == ENOENT)
                //                {
                perror("Execv failed: Incorrect invalid argument\n");
                exit(EXIT_FAILURE);
                //                }
            }
        }
    }
}

#pragma GCC diagnostic pop

static void socket_close(int sockfd)
{
    if(close(sockfd) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
}

int tokenizeString(char *string, const char *delimiter, char *array[])
{
    int   arraySize = 0;
    char *token;
    char *rest = string;

    while((token = strtok_r(rest, delimiter, &rest)))
    {
        //        printf("Path: %s", token);
        array[arraySize] = token;
        arraySize++;
    }

    array[arraySize] = NULL;
    return arraySize;
}
