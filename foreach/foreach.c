#include <bufio.h>
#include <helpers.h>

#define BUFFER_SIZE 4096

int main(int argc, char * argv[]) {
    if (argc < 2) 
        exit(EXIT_FAILURE);
    
    struct buf_t * buf = buf_new(BUFFER_SIZE); 
    char string[BUFFER_SIZE];
    char * args[argc + 1];
    for (int i = 1; i < argc; ++i)
        args[i - 1] = argv[i];
    args[argc] = 0;
    ssize_t string_size = 0; 
    while ((string_size = buf_getline(STDIN_FILENO, buf, string)) != -1) {
        if (string_size % 2 == 0) {
            args[argc - 1] = string;
            if (spawn(args[0], args)) {
                continue;
            }
            string[string_size] = '\n';
            string[string_size + 1] = 0;
            write_(STDOUT_FILENO, string, string_size + 1);
        }
    }
    buf_free(buf);
    exit(EXIT_SUCCESS);
}
