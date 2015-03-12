#include <unistd.h>
#include <stdlib.h>
#include "helpers.h"

#define BUFFER_SIZE 2048 

int main() 
{
    char buffer[BUFFER_SIZE];
    for (;;) 
    {
        ssize_t read_block = read_(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (read_block < 0)
        {
            exit(EXIT_FAILURE);
        }
        ssize_t write_block = write_(STDOUT_FILENO, buffer, read_block);
        if (write_block < 0) 
        {
            exit(EXIT_FAILURE);
        }
        if (write_block == 0)
        {
            break;
        }
    }
    exit(EXIT_SUCCESS);
}
