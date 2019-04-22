/* 序列化演示
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

typedef struct{
    int id;
    int sex;
    int age;
    char name[32];
} stu;

int main(){
    stu s;
    s.id = 10;
    s.age = 23;
    s.sex = 1;
    strcpy(s.name, "xiaoming");
    int a = 100;
    int c = 200;
    char buff[1024] = {0};
    sprintf(buff, "a=%d;c=%d", a, c);

    // 打开一个文件，如果文件不存在则创建文件，给文件权限
    int fd = open("./tmp.txt", O_CREAT | O_RDWR, 0664);
    if(fd < 0){
        printf("open error\n");
        return -1;
    }

    // 写入数据
    write(fd, (void*)&s, sizeof(stu));
    
    // 读出文件中的数据
    lseek(fd, 0, SEEK_SET);
    stu child;
    read(fd, (void*)&child, sizeof(stu));
    printf("name:%s-id:%d age:%d sex:%d\n", child.name, child.id, child.age, child.sex);

    // int b = -1;
    // read(fd, (void*)&b, sizeof(int));
    // printf("%d\n", b);

    close(fd);
    return 0;
}
