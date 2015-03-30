#include "bufio.h"

#define BUFFER_CAPACITY 2048

int main()
{
    struct buf_t * buf = buf_new(BUFFER_CAPACITY);
    for (;;) 
    {
        size_t required = buf_capacity(buf) - buf_size(buf);
        ssize_t curr_size = buf_fill(STDIN_FILENO, buf, required);
        if (curr_size < required)
        {
            if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) == -1 || curr_size == -1)
            {
                buf_free(buf);
                exit(EXIT_FAILURE);
            }
            buf_free(buf);
            break;
        }
        if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) == -1)
        {
            buf_free(buf);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
} 

