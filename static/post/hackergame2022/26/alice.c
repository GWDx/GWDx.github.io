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
    char secret[100];
    // open file /secret and read
    FILE* fp = fopen("/secret", "r");
    fgets(secret, 100, fp);

    int shmid = shmget(key, BUFSZ, IPC_CREAT | 0666);  // 创建共享内存
    char* shmadd = shmat(shmid, NULL, 0);              // 映射
    bzero(shmadd, BUFSZ);                              // 共享内存清空
    strcpy(shmadd, secret);

    return 0;
}
