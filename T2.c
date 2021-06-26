// 通过 fork/exec/pipe/dup 实现 cat </etc/passwd | wc -l >result.txt 

// + 父进程创建子进程
//   - 在父进程中实现功能 cat </etc/passwd
//   - 在子进程中实现功能 wc -l >result.txt
//   - 不能实现为
//     * 在子进程中实现功能 cat </etc/passwd
//     * 在父进程中实现功能 wc -l >result.txt

// + 该题不要求实现一个通用的 shell 程序
//   - 不需要使用 strtok 对字符串进行分割处理
//   - 假设字符串已经分割处理好了
//     * 父进程，使用 execlp("cat") 执行命令，使用 open("/etc/passwd") 打开文件
//     * 子进程，使用 execlp("wc") 执行命令，使用 open("result.txt") 打开文件

// + 请严格按要求完成
//   - 把作业 sh3.c 的内容复制过来，是没有分数的

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
   int pid;
   int fd[2];
   int file;
   
   char buf[32];
   pipe(fd);
   pid=fork();
   if(pid==0)
   {
    file=open("result.txt",O_CREAT|O_RDWR,666);
    dup2(fd[0],0);
    dup2(file,1);
    close(fd[0]);
    close(fd[1]);
    close(file);
    read(0,buf,sizeof(buf));
    execlp("wc","wc","-l",NULL);
    printf("%s",buf);
   }


    file=open("/etc/passwd",O_RDONLY);
    dup2(fd[1],1);
    dup2(file,0);
    close(fd[0]);
    close(fd[1]);
    execlp("cat","cat","/etc/passwd",NULL);
    exit(0);
     
      
   return 0;
}