#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFSZ 512

int main() {
    key_t key = 0x1234;
    int shmid = shmget(key, BUFSZ, IPC_CREAT | 0666);  // 打开共享内存
    char* shmadd = shmat(shmid, NULL, 0);              // 映射
    printf("%s", shmadd);                              // 读共享内存区数据
    int ret = shmdt(shmadd);                           // 分离共享内存和当前进程
    shmctl(shmid, IPC_RMID, NULL);                     // 删除共享内存
    return 0;
}
