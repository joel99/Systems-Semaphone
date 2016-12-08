#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

main(int argc, char* argv[]){

  union semun {
    int val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
  };

  union semun sc;
  int key = ftok("makefile", 10);
  
  if(argv[1] == NULL){
    return;
  }
  
  else if (strncmp(argv[1], "-c", strlen(argv[1])) == 0 && argv[2] != NULL ){
    
    //Create a semaphore
    int sd = semget(key, 1, IPC_CREAT | 0644);
    if (sd > 0){
      printf("Created a semaphore: %d\n", sd);
      int j = atoi(argv[2]);
      sc.val = j;
      printf("Semaphore's value: %d\n", j);
      semctl(sd, 0, SETVAL, sc);
      printf("Set the semaphore's value: %d\n", j);
    }
    
    int fd = open("story.txt", O_CREAT | O_TRUNC | O_WRONLY | 0644);

    int shmid = shmget(key, 100, IPC_CREAT | IPC_EXCL);
     
  }
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){
    int sd = semget(key, 1, IPC_CREAT | 0644);
    int x = semctl(sd, 0, GETVAL);
    printf("Semaphore's value: %d\n", x);
  }

  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0){
    int sd = semget(key, 1, IPC_CREAT | 0644);
    int x = semctl(sd, 0, IPC_RMID, 0);
    printf("Removed semaphore\n");
  }

  else{return;}

  
}
