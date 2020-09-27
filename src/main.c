#include <stdio.h>

#include "tcp_client.h"

int main(int argc, char *argv[]) 
{
    FILE *fileOpen;
    char* action = malloc(4 * sizeof(char*));
    /*for(int i = 0; i < 4;i++)
    {   
        action[i] = (char *)malloc (10 * sizeof (char));
    }*/
    char* message = malloc(4 * sizeof(char*));
    printf("Arc 0, %d \n", argc); 
    //printf("Arg 0, %s \n", argv[0]);
    //printf("Arg 1, %s \n", argv[1]);
        printf("Arg 2, %s \n", "input.txt");
    fileOpen = tcp_client_open_file(argv[1]);
    tcp_client_get_line(fileOpen,&action, &message);
    /*while (EOF != (ch = fgetc(fp)))
   // do something

if (ferror(fp) != 0)
   // error handling*/
    return 0; 
}
