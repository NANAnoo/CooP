#include <libc.h>
#include <stdio.h>

const char *fd_name = "test_fifo";
const int BUFF_SIZE = 1024;
int main() {
    int pipe = 0;
    char buff[BUFF_SIZE];
    while (pipe != -1 && buff[0] != '.') {
        pipe = open(fd_name, O_RDONLY);
        size_t len = read(pipe, buff, BUFF_SIZE);
        write(STDOUT_FILENO, buff, len);
        close(pipe);
    }
    return 0;
}