//
// Created by ZhangHao on 2023/8/16.
//

#include <libc.h>

int main(void){
    int fd[2];
    char buf[1024];
    pid_t pid;

    //fd[0] 读端
    //fd[1] 写端
    if(pipe(fd)<0){  //在此处调用pipe函数创建管道
        perror("pipe");
        exit(1);
    }
    pid=fork();      //创建子进程

    //父写子读
    if(pid>0){            //pid>0说明运行此程序的是父进程
        //父进程里，关闭父读
        close(fd[0]);
        while(buf[0] != '.') {
            read(STDIN_FILENO, buf, 1);
            write(fd[1],buf,1);
        }
    }else if(pid==0){    //pid=0说明运行此程序的是子进程
        //子进程里，关闭子写
        close(fd[1]);
        size_t len;
        while (buf[0] != '.') {
            len= read(fd[0],buf,1);
            write(STDOUT_FILENO,buf,len);
        }
    }else{
        perror("fork");
        exit(1);
    }
}