#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * argv[]){

  int fd = open("story.txt", O_CREAT | O_RDWR | O_APPEND);

  int key = ftok("makefile", 10);

  int semid = semget(key, 1, 0);

  //can we do this:
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  
  semop(semid, &sb, 1);

  int shmid = shmget(key, 100, IPC_EXCL);
  int * shm;
  shmat(shmid, shm, 0); //attach
  char * lastLine;
  //set fd offset to last newline
  lseek(fd, *shm, SEEK_SET);
  //read lastLine
  read(fd, lastLine, 100);
  printf("The last line was: \n %s", lastLine);
  //store back into shm
  
  printf("Please enter your addition: \n");
  char read[256];
  char * s = read;
  fgets(read, sizeof(read), stdin);

  printf("Well that's done\n");
  lseek(fd, 0, SEEK_END); //reposition file offset
  write(fd, s, 256);//write to file
  //get new offset
  * shm = lseek(fd, 0, SEEK_CUR);

  //great!
  shmdt(shm);
  close(fd);

  //up the sem
  sb.sem_op = 1;
  semop(semid, &sb, 1);

}
