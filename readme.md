# 2021 nuaa os exam

## T1
+ 实现函数 list_visit
  - 主程序遍历链表 list 的每个节点
  - 对每个节点创建一个子线程，在这道题目中，总共创建 7 个子线程
  - 在每个子线程中，打印 node->data

+ 实现函数 wait_sub_threads
  - 等待所有的子线程执行完毕
  - 在 list_visit 时，需要记录下所有线程的 ID，记录在全局变量中
  - 在 wait_sub_threads 中使用 pthread_join

+ 程序可能的输出结果
  - 字符 a b c d e f g 每个出现一次，出现的顺序可能是随机的  
  - END 一定是最后出现
  a
  d
  b
  e
  c
  f
  g
  END

> 不会,现场搜pthread_create定义乱写的
```c
// + 实现函数 list_visit
//   - 主程序遍历链表 list 的每个节点
//   - 对每个节点创建一个子线程，在这道题目中，总共创建 7 个子线程
//   - 在每个子线程中，打印 node->data

// + 实现函数 wait_sub_threads
//   - 等待所有的子线程执行完毕
//   - 在 list_visit 时，需要记录下所有线程的 ID，记录在全局变量中
//   - 在 wait_sub_threads 中使用 pthread_join

// + 程序可能的输出结果
//   - 字符 a b c d e f g 每个出现一次，出现的顺序可能是随机的  
//   - END 一定是最后出现
//   a
//   d
//   b
//   e
//   c
//   f
//   g
//   END
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<pthread.h>
struct node {
    char data;
    struct node *next;
};
pthread_t tid[7];
void show(struct node *node);

struct node *node_create(char data)
{
    struct node *node = malloc(sizeof(struct node));
    node->data = data;
    node->next = NULL;
    return node;
}

void list_visit(struct node *list)
{
    struct node * tmp = list;
    int cnt = 0;
    while(tmp != NULL)
    {
       pthread_create(&tid,NULL,show,tmp);
        cnt++;
        tmp = tmp->next;
    }

    
}
void show(struct node *node)
{
    printf("%c\n", node->data);
    return;
}
// 遍历算法，供参考
void list_print(struct node *list)
{
    struct node *node = list;
    while (node) {
        
        node = node->next;
    }
}

void wait_sub_threads()
{
    for(int i =0;i<7;++i)
        pthread_join(tid[i],NULL);
}

int main()
{
    struct node *list = NULL;
    for (char data = 'g'; data >= 'a'; data--) {
        struct node *node = node_create(data);
        node->next = list;
        list = node;
    }
    list_visit(list);
    // list_print(list);
    wait_sub_threads();

    puts("END");
    return 0;
}

```
## T2
通过 fork/exec/pipe/dup 实现 cat </etc/passwd | wc -l >result.txt 

+ 父进程创建子进程
  - 在父进程中实现功能 cat </etc/passwd
  - 在子进程中实现功能 wc -l >result.txt
  - 不能实现为
    * 在子进程中实现功能 cat </etc/passwd
    * 在父进程中实现功能 wc -l >result.txt

+ 该题不要求实现一个通用的 shell 程序
  - 不需要使用 strtok 对字符串进行分割处理
  - 假设字符串已经分割处理好了
    * 父进程，使用 execlp("cat") 执行命令，使用 open("/etc/passwd") 打开文件
    * 子进程，使用 execlp("wc") 执行命令，使用 open("result.txt") 打开文件

+ 请严格按要求完成
  - 把作业 sh3.c 的内容复制过来，是没有分数的

> 原题 抄的[这里](https://github.com/Sk-Raven/NUAA-OSLAB-2019/blob/master/exam/t1.c),有细微改动,父子进程要求变了. 考试的时候不知道什么是子进程,所以没交换代码顺序,实际上直接交换一下if里面内容即可.


```c
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
    //考完了才知道这是子进程
    //交换这两块内容
   }


    file=open("/etc/passwd",O_RDONLY);
    dup2(fd[1],1);
    dup2(file,0);
    close(fd[0]);
    close(fd[1]);
    execlp("cat","cat","/etc/passwd",NULL);
    exit(0);
    //交换这两块内容
    
   return 0;
}
```

## T3
+ 补全生产者、计算者、消费者程序
  - 完成函数 buffer_init
  - 完成函数 buffer_get
  - 完成函数 buffer_put
  - 补全函数 main
    * 创建生产者、计算者、消费者

+ 程序最终输出
  - 生产者输出 a b c d e f g h
  - 消费者输出 A B C D E F G H
  - 消费者输出 A B C D E F G H
  - 两者交叉输出 

+ 不准对函数 produce 、函数 compute、 函数 consume 进行任何改动
  - 如果改动，则该题没有分数

+ 本题对 buffer 进行了抽象和封装
  - buffer_pc 连接 producer 和 computer
  - buffer_cc 连接 computer 和 consumer


> 原题 对着抄的pc1改,有手就行;反正让我自己写肯定不会

```c
// + 补全生产者、计算者、消费者程序
//   - 完成函数 buffer_init
//   - 完成函数 buffer_get
//   - 完成函数 buffer_put
//   - 补全函数 main
//     * 创建生产者、计算者、消费者

// + 程序最终输出
//   - 生产者输出 a b c d e f g h
//   - 消费者输出 A B C D E F G H
//   - 消费者输出 A B C D E F G H
//   - 两者交叉输出 

// + 不准对函数 produce 、函数 compute、 函数 consume 进行任何改动
//   - 如果改动，则该题没有分数

// + 本题对 buffer 进行了抽象和封装
//   - buffer_pc 连接 producer 和 computer
//   - buffer_cc 连接 computer 和 consumer 

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

#define CAPACITY 4      // 缓冲区的最大容量

struct buffer {
    int data[CAPACITY];
    int in;             // 缓冲区的写指针
    int out;            // 缓冲区的读指针
    pthread_mutex_t mutex;
    pthread_cond_t wait_empty_buffer;
    pthread_cond_t wait_full_buffer;
};

// 初始化 buffer，需要初始化用于同步的字段 
void buffer_init(struct buffer *buffer)
{
    buffer->in = 0;
    buffer->out = 0;
    pthread_mutex_init(&buffer->mutex,NULL);
    pthread_cond_init(&buffer->wait_empty_buffer, NULL);
    pthread_cond_init(&buffer->wait_full_buffer , NULL);
}

// 判断缓冲区是否为空
int buffer_is_empty(struct buffer *buffer)
{
    return buffer->in == buffer->out;
}

// 判断缓冲区是否为满
int buffer_is_full(struct buffer *buffer)
{
    return (buffer->in + 1) % CAPACITY == buffer->out;
}

// 向缓冲区中追加一个数据，如果缓冲区为满，则等待
void buffer_put(struct buffer *buffer, int item)
{
    pthread_mutex_lock(&buffer->mutex);
    
    while(buffer_is_full(buffer))
        pthread_cond_wait(&buffer->wait_empty_buffer,&buffer->mutex);
    buffer->data[buffer->in] = item;
    buffer->in = (buffer->in + 1) % CAPACITY;

    pthread_cond_signal(&buffer->wait_full_buffer);
    pthread_mutex_unlock(&buffer->mutex);
}

// 从缓冲区中取走一个数据，如果缓冲区为空，则等待
int buffer_get(struct buffer *buffer)
{
    pthread_mutex_lock(&buffer->mutex);
    int item;
    while(buffer_is_empty(buffer))
        pthread_cond_wait(&buffer->wait_full_buffer,&buffer->mutex);
    item = buffer->data[buffer->out];
    buffer->out = (buffer->out + 1) % CAPACITY;
    pthread_cond_signal(&buffer->wait_empty_buffer);
    pthread_mutex_unlock(&buffer->mutex);
    return item;
}

#define ITEM_COUNT (2 * CAPACITY)

struct buffer buffer_pc, buffer_cc;

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        item = 'a' + i;
        printf("produce item: %c\n", item);
        buffer_put(&buffer_pc, item);
    }
    return NULL;
}

void *compute(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        item = buffer_get(&buffer_pc); 
        item = toupper(item);
        printf("    compute item: %c\n", item); 
        buffer_put(&buffer_cc, item);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        item = buffer_get(&buffer_cc); 
        printf("        consume item: %c\n", item); 
    }
    return NULL;
}

int main()
{ 
    int i;
    buffer_init(&buffer_pc);
    buffer_init(&buffer_cc);
    pthread_t tids[3];
    pthread_create(&tids[0],NULL,produce,NULL);
    pthread_create(&tids[1],NULL,compute,NULL);
    pthread_create(&tids[2],NULL,consume,NULL);
    for(i = 0;i < 3;i++)
        pthread_join(tids[i],NULL);
    pthread_mutex_destroy(&buffer_pc.mutex);
    pthread_mutex_destroy(&buffer_pc.mutex);

    return 0;
}

```
顺便附上pc1 也是抄的
```c
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#define CAPACITY 4

char buffer1[CAPACITY];
char buffer2[CAPACITY];
int in1,out1;
int in2,out2;

int buffer_is_empty(int index){
    if(index == 1)
        return in1 == out1;
    if(index == 2)
        return in2 == out2;
    else
        printf("Don`t exist this buffer!,Empty");
}

int buffer_is_full(int index){
    if(index == 1)
        return (in1 + 1) % CAPACITY == out1;
    if(index == 2)
        return (in2 + 1) % CAPACITY == out2;
    else
        printf("Don`t exist this buffer!,Full");
}
char get_item(int index){
    char item;
    if(index == 1){
        item = buffer1[out1];
        out1 = (out1 + 1) % CAPACITY;
    }
    if(index == 2){
        item = buffer2[out2];
        out2 = (out2 + 1) % CAPACITY;
    }
    //else
    //  printf("Don`t exist this buffer!,Get%d\n",index);
    return item;
}

void put_item(char item, int index){
    if(index == 1){
        buffer1[in1] = item;
        in1 = (in1 + 1) % CAPACITY;
    }
    if(index == 2){
        buffer2[in2] = item;
        in2 = (in2 + 1) % CAPACITY;
    }
    //else
    //    printf("Don`t exist this buffer!Put%c  %d\n",item,index);
}

pthread_mutex_t mutex1,mutex2;
pthread_cond_t wait_empty_buffer1;
pthread_cond_t wait_full_buffer1;
pthread_cond_t wait_empty_buffer2;
pthread_cond_t wait_full_buffer2;


volatile int global = 0;

#define ITEM_COUNT 8

void *produce(void *arg){
    int i;
    char item;

    for(i = 0;i < ITEM_COUNT;i++){
        pthread_mutex_lock(&mutex1);
        while(buffer_is_full(1))
            pthread_cond_wait(&wait_empty_buffer1, &mutex1);
        item = 'a' + i;
        put_item(item,1);
        printf("produce item:%c\n",item);

        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}
void *compute(void *arg){
    int i;
    char item;
    for(i = 0;i < ITEM_COUNT;i++){
        pthread_mutex_lock(&mutex1);
        while(buffer_is_empty(1))
            pthread_cond_wait(&wait_full_buffer1, &mutex1);
        item = get_item(1);
        //printf("    compute get item:%c\n",item);
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex1);

        item -= 32;

		pthread_mutex_lock(&mutex2);
        while(buffer_is_full(2))
            pthread_cond_wait(&wait_empty_buffer2, &mutex2);
        put_item(item,2);
        printf("    compute put item:%c\n", item);
        pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

void *consume(void *arg){
    int i;
    char item;
    for(i = 0;i < ITEM_COUNT;i++){
        pthread_mutex_lock(&mutex2);
        while(buffer_is_empty(2))
            pthread_cond_wait(&wait_full_buffer2, &mutex2);
        item = get_item(2);
        printf("            comsume item:%c\n", item);

        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

int main(){
    int i;
    in1 = 0;
    in2 = 0;
    out1 = 0;
    out2 = 0;
    pthread_t tids[3];
	pthread_create(&tids[0],NULL,produce,NULL);
    pthread_create(&tids[1],NULL,compute,NULL);
    pthread_create(&tids[2],NULL,consume,NULL);

    pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_buffer1, NULL);
    pthread_cond_init(&wait_full_buffer1, NULL);
    pthread_cond_init(&wait_empty_buffer2, NULL);
    pthread_cond_init(&wait_full_buffer2, NULL);

    for(i = 0;i < 3;i++)
        pthread_join(tids[i],NULL);
    pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);


    return 0;
}
```

# T4
实现命令 mygrep string file
+ 该命令逐行读取文件，如果行包括目标字符串，则打印该行
+ 该命令有两个命令行参数
  - 参数 string，要搜索的字符串
  - 参数 file，要查找的文件名

例子，在文件 /etc/passwd 中查找字符串 root，打印包含该字符串的行
```
$ ./mygrep root /etc/passwd
root:x:0:0:root:/root:/bin/bash
```
提示，可以使用函数 strstr 在字符串中查找字符串
https://www.runoob.com/cprogramming/c-function-strstr.html


> 照着之前抄的作业改的;[readline从这抄的](https://github.com/FlyAndNotDown/nuaa-os-lab/blob/master/2-multi-process/2-sh1/sh1.c) 让我自己写肯定写不出来 

```c
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
```
