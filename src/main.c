#include <stdio.h>

#include "tcp_client.h"

int main(int argc, char *argv[]) 
{
    FILE *fileOpen;
    char* action[10];
    char* message[1024];
    printf("Arc 0, %d \n", argc); 
    printf("Arg 0, %s \n", argv[0]);
    printf("Arg 1, %s \n", argv[1]);
        printf("Arg 2, %s \n", argv[2]);
    fileOpen = tcp_client_open_file(argv[1]);
    tcp_client_get_line(fileOpen,action, message);
    /*while (EOF != (ch = fgetc(fp)))
   // do something

if (ferror(fp) != 0)
   // error handling*/
    return 0; 
}
