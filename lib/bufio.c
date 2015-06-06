#include <string.h>
#include "bufio.h"

struct buf_t * buf_new(size_t capacity) {
    struct buf_t * buf = (struct buf_t * ) malloc(sizeof(struct buf_t));
    if (buf == NULL)
        return NULL;
    buf->buffer = (char *) malloc(sizeof(char) * capacity);
    if (buf->buffer == NULL)
        return NULL;
    buf->capacity = capacity;
    buf->size = 0;
    buf->finish = 0;
    return buf;
}

void buf_free(struct buf_t * buf) {
    if (buf != NULL) {
        free(buf->buffer);
        free(buf);
    }
}

int buf_finish(struct buf_t * buf) {
#ifdef DEBUG
    if (buf == NULL) 
        abort();
#endif
    return buf->finish;
}

size_t buf_capacity(struct buf_t * buf) {
#ifdef DEBUG
    if (buf == NULL) 
        abort();
#endif
    return buf->capacity;
}

size_t buf_size(struct buf_t * buf) {
#ifdef DEBUG
    if (buf == NULL) 
        abort();
#endif
    return buf->size;
}

ssize_t buf_fill(int fd, struct buf_t * buf, size_t required) {
#ifdef DEBUG
    if (buf == NULL) 
        abort();
#endif
    if (required > buf->capacity)
        return -1;
    while (buf->size < required) {
        ssize_t read_block = read(fd, buf->buffer + buf->size, required - buf->size);
        if (read_block == -1)
            return -1;
        if (read_block == 0) {
            buf->finish = 1;
            break;
        }
        buf->size += read_block;
    }
    return buf->size;
}

ssize_t buf_flush(int fd, struct buf_t * buf, size_t required) {
#ifdef DEBUG
    if (buf == NULL) 
        abort();
#endif
    size_t prev_size = buf->size;
    required = required < buf->size ? required : buf->size; // 100% <= buf->size
    while (required > 0) {
        ssize_t write_block = write(fd, buf->buffer, required);
        if (write_block == -1) 
            return -1;
        memmove(buf->buffer, buf->buffer + write_block, buf->size - write_block);
        required -= write_block;
        buf->size -= write_block;
    }
    return prev_size - buf->size;
}

ssize_t buf_getline(int fd, struct buf_t * buf, char * dest)
{
#ifdef DEBUG 
    if (buf == NULL) 
        abort();
#endif
    size_t str_len = 0;
    for(;;)
    {
        for (size_t i = 0; i != buf->size; ++i)
        {
            if (buf->buffer[i] == '\n')
            {
                memmove(buf->buffer, buf->buffer + i + 1, buf->size - i - 1);
                buf->size -= i + 1;
                return str_len;
            }
            dest[str_len++] = buf->buffer[i];
        }
        if (buf_fill(fd, buf + buf->size, 1) <= 0)
        {
            return -1;
        }
    }
    return -1;
}
