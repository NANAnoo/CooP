#include <libc.h>
#include <errno.h>
#include <string.h>

extern int errno ;

const char *fd_name = "./test_fifo";
const int BUFF_SIZE = 1024;
int main() {
    int fifo = open(fd_name, O_WRONLY);
    if (fifo == -1) {
        mkfifo(fd_name, S_IWUSR|S_IWGRP|S_IWOTH|S_IRUSR|S_IRGRP|S_IROTH);
        fifo = open(fd_name, O_WRONLY);
    }
    char buff[BUFF_SIZE];
    printf("pipe connected \n");
    while(fifo != -1 && buff[0] != '.') {
        size_t len = read(STDIN_FILENO, buff, BUFF_SIZE);
        write(fifo, buff, len);
        close(fifo);
        fifo = open(fd_name, O_WRONLY | O_NONBLOCK);
    }
    if (errno)
        fprintf(stderr, "Error creating fifo file: %s\n", strerror( errno ));
    return errno;
}