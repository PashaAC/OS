#ifndef HELPERS_H_
#define HELPERS_H_

ssize_t read_(int fd, void * buf, size_t count);
ssize_t write_(int fd, const void * buf, size_t count);
ssize_t read_until(int fd, void * buf, size_t count, char delimiter);

#endif
