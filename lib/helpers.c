#include <unistd.h>
#include "helpers.h"

ssize_t read_(int fd, void * buf, size_t count)
{
    size_t offset = 0;
    while (offset < count) 
    {
        ssize_t read_block = read(fd, buf + offset, count - offset);
        if (read_block < 0) 
        {
            return read_block;
        }
        if (read_block == 0) 
        {
            break;
        }
        offset += read_block;
    }
    return offset;
}

ssize_t write_(int fd, const void * buf, size_t count) 
{
    size_t offset = 0;
    while (offset < count) 
    {
        ssize_t write_block = write(fd, buf + offset, count - offset);
        if (write_block < 0) 
        {
            return write_block;
        }
        offset += write_block;
    }
    return offset;
}

ssize_t read_until(int fd, void * buf, size_t count, char delimiter) 
{    
    size_t offset = 0;
    while (offset < count) 
    {
        ssize_t read_block = read(fd, buf + offset, count - offset);
        if (read_block < 0) 
        {
            return read_block;
        }
        if (read_block == 0) 
        {
            break;
        }
        for (size_t i = offset; i != offset + read_block; ++i)
        {
            if (((char *)buf)[i] == delimiter)
            {
                return offset + read_block;
            }
        }
        offset += read_block;
    }
    return offset;
}
