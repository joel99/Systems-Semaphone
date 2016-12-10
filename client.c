#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>

int main(int argc, char * argv[]){

  int fd = open("story.txt", O_CREAT | O_RDWR | O_APPEND);

  int key = ftok("makefile", 10);

  int semid = semget(key, 1, 0);

  //can we do this:
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  printf("Requesting access to story, please hold.\n");
  semop(semid, &sb, 1);
  printf("Access granted.\n");
  
  int shmid = shmget(key, 100, 0644);
  int * shm;
  shm = (int *)shmat(shmid, 0, 0); //attach
  char lastLine[(*shm) + 1];
  
  //set fd offset to last newline
  
  lseek(fd, -(*shm), SEEK_END);
  //read lastLine
  int readLen = read(fd, lastLine, *shm);
  //terminate the string
  lastLine[readLen] = 0;
  if (!readLen)
    printf("You are the first contributor.\n");
  else {
    printf("The last message was: \n\n\t%s\n", lastLine);
  }
  //store back into shm
  
  printf("Please enter your addition:\t");
  char msg[256];
  fgets(msg, sizeof(msg), stdin);

  * shm = strlen(msg);
  
  //great!
  
  //lseek(fd, 0, SEEK_END);
  write(fd, msg, * shm);//write to file
  close(fd);

  shmdt(shm);
  //up the sem
  sb.sem_op = 1;
  semop(semid, &sb, 1);

  return 0;
}
