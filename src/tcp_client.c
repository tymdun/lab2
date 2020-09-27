#include "log.h"

#include "tcp_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TCP_CLIENT_BAD_SOCKET -1
#define TCP_CLIENT_DEFAULT_PORT "8081"
#define TCP_CLIENT_DEFAULT_HOST "localhost"


// Parses the options given to the program. It will return a Config struct with the necessary
// information filled in. argc and argv are provided by main. If an error occurs in processing the
// arguments and options (such as an invalid option), this function will print the correct message
// and then exit.
Config tcp_client_parse_arguments(int argc, char *argv[]);

////////////////////////////////////////////////////
///////////// SOCKET RELATED FUNCTIONS /////////////
////////////////////////////////////////////////////

// Creates a TCP socket and connects it to the specified host and port. It returns the socket file
// descriptor.
int tcp_client_connect(Config config)
{    int sockfd;                   // tcp socket
    int portno;                   // Holds the port number
    struct sockaddr_in serv_addr; // Contains info to connect to server
    struct hostent *server;       // Information recieved from the server
    portno = atoi(config.port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        log_info("ERROR opening socket");
    } else {
        log_info("Socket successfully opened");
    }

    server = gethostbyname(config.host);
    if (server == NULL) {
        log_trace("ERROR, no such host");
        fprintf(stderr, "Error, no such host\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr)); // Clears the memory used by the serv_addr struct
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr,
           server->h_length); // Copies the server informmation to use for the client
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        log_trace("ERROR connecting");
        exit(EXIT_FAILURE);
    } else {
        log_trace("Connection Successful");
    }

    if (sockfd == TCP_CLIENT_BAD_SOCKET) {
        exit(EXIT_FAILURE);
    }

    return sockfd;}

// Using the the action and message provided by the command line, format the data to follow the
// protocol and send it to the server. Return 1 (EXIT_FAILURE) if an error occurs, otherwise return
// 0 (EXIT_SUCCESS).
int tcp_client_send_request(int sockfd, char *action, char *message);

// Receive the response from the server. The caller must provide a function pointer that handles the
// response and returns a true value if all responses have been handled, otherwise it returns a
// false value. After the response is handled by the handle_response function pointer, the response
// data can be safely deleted. The string passed to the function pointer must be null terminated.
// Return 1 (EXIT_FAILURE) if an error occurs, otherwise return 0 (EXIT_SUCCESS).
int tcp_client_receive_response(int sockfd, int (*handle_response)(char *));

// Close the socket when your program is done running.
void tcp_client_close(int sockfd){
    log_trace("Closing socket");
    close(sockfd);
    log_info("Socket closed");
}

////////////////////////////////////////////////////
////////////// FILE RELATED FUNCTIONS //////////////
////////////////////////////////////////////////////

// Given a file name, open that file for reading, returning the file object.
FILE *tcp_client_open_file(char *file_name)
{
    FILE *openFile; 
    openFile = fopen(file_name, "r"); 
    if(openFile == NULL)
    {
        log_fatal("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    return openFile;
}

// Gets the next line of a file, filling in action and message. This function should be similar
// design to getline() (https://linux.die.net/man/3/getline). *action and message must be allocated
// by the function and freed by the caller.* When this function is called, action must point to the
// action string and the message must point to the message string.
int tcp_client_get_line(FILE *fd, char **action, char **message)
{
    size_t messageArraySize = (sizeof(*message) / sizeof(char));
    size_t currentCharCount = 0; 
    int32_t *allocateSuccess; 
    printf("%ld, %ld, did we make it\n", messageArraySize, currentCharCount);
    fscanf(fd, "%s", *action);
    //strncpy(*action, fd, sizeof(*action)); 
    log_trace("Action to be performed: %s", *action); 
    while(true)
    {
        printf("here\n");
        if (messageArraySize <= currentCharCount)
        {
            printf("entered\n");
            printf("current memory size %ld\n", messageArraySize);
            allocateSuccess = realloc(*message, (messageArraySize * 2));
            if(allocateSuccess == NULL)
            {
                log_debug("Failure to reallocate successfully");
                exit(EXIT_FAILURE);
            }
            messageArraySize = (sizeof(*message) / sizeof(char));
            printf("current memory size %ld\n", messageArraySize);
        }
        printf("now\n");
        *message[currentCharCount] = fgetc(fd);
        printf("oof\n");
        if(*message[currentCharCount] == '\n')
        {
            currentCharCount++;
            printf("biiger oof\n"); 
            log_trace("Message to send: %s", *message);
            printf("Message to send: %s", *message);
            return 1; 
        }
        currentCharCount++;
        printf("current char count %ld\n", currentCharCount);
        printf("current memory size %ld\n", messageArraySize);
    }
}

// Close the file when your program is done with the file.
void tcp_client_close_file(FILE *fd)
{
    fclose(fd);
}

