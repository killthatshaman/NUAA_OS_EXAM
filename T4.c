// 实现命令 mygrep string file
// + 该命令逐行读取文件，如果行包括目标字符串，则打印该行
// + 该命令有两个命令行参数
//   - 参数 string，要搜索的字符串
//   - 参数 file，要查找的文件名

// 例子，在文件 /etc/passwd 中查找字符串 root，打印包含该字符串的行
// $ ./mygrep root /etc/passwd
// root:x:0:0:root:/root:/bin/bash

// 提示，可以使用函数 strstr 在字符串中查找字符串
// https://www.runoob.com/cprogramming/c-function-strstr.html

#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
int readline(char *buf, int len,int file) 
{
    char c[2];
    c[1] = '\x00';
    int count = 0;
    read(file,c,1);
    while (c[0] != '\n') 
    {
        count++;
        if (count < len - 1) 
        {
            buf[count - 1] = c[0];
        } 
        else 
            return 0;
        read(file,c,1);

    }
    buf[count] = '\0';
    return 1;
}
int main(int argc ,char * argv[])
{
    int file;
    file = open(argv[2],O_RDONLY);
    char buf[1024];
    while(readline(buf,1000,file))
    {
        if(strstr(buf,argv[1]))
        {
            puts(buf);
        }
    }
    close(file);
    return 0;
}