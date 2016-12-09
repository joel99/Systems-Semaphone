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

void main(int argc, char* argv[]){

  union semun {
    int val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
  };

  //Creating the semun for later use
  union semun sc;
  
  //Creating a key
  int key = ftok("makefile", 10);

  //Ending the program when no command is given
  if(argv[1] == NULL){
    printf("No command given so nothing done! Refer to readme.\n");
    return;
  }

  //The -c command
  else if (strncmp(argv[1], "-c", strlen(argv[1])) == 0 ){
    
    //Creating a semaphore
    int sd = semget(key, 1, IPC_CREAT | 0644);
    if (sd > 0){
      printf("Created a semaphore with id: %d\n", sd);
      sc.val = 1;
      semctl(sd, 0, SETVAL, sc);
    }
    
    else{
      printf("Semaphore Creation Failed!");
    }

    //Open the story.txt file and create if not there
    int fd = open("story.txt", O_CREAT | O_TRUNC | O_WRONLY | 0644);
    close(fd);

    //Creating shared memory
    int shmid = shmget(key, 100, IPC_CREAT);
    if (shmid > 0){
      printf("Created a shared memory with id: %d\n", shmid);
    }
    else{
      printf("Shared Memory Creation Failed!");
    }
  
  }

  //The -v command
  else if (strncmp(argv[1], "-v", strlen(argv[1])) == 0){

    //Displaying the contents of the file
    printf("Printing out the text in story.txt: \n");
    int fd = open("story.txt", O_RDONLY);
    struct stat info;
    fstat(fd, &info);
    int size = info.st_size;
    if (size > 0){
      char buf[size];
      read(fd, buf, size);
      printf("%s", buf);
      printf("End of story.txt\n");
    }
    else{
      printf("There is no text in story.txt\n");
    }
    close(fd);
    
  }

  //The -r command
  else if (strncmp(argv[1], "-r", strlen(argv[1])) == 0){

    //Removing the semaphore
    int sd = semget(key, 1, IPC_CREAT | 0644);
    semctl(sd, 0, IPC_RMID, 0);
    printf("Removed semaphore\n");

    //Removing the shared memory
    int shmid = shmget(key, 100, IPC_CREAT);
    shmctl(shmid, IPC_RMID);
    printf("Removed shared memory\n");
    
    //Displaying the contents of the file
    printf("Printing out the text in story.txt: \n");
    int fd = open("story.txt", O_RDONLY);
    struct stat info;
    fstat(fd, &info);
    int size = info.st_size;
    if (size > 0){
      char buf[size];
      read(fd, buf, size);
      printf("%s", buf);
      printf("End of story.txt\n");
    }
    else{
      printf("There is no text in story.txt\n");
    }
    close(fd);
    
  }

  //Final else
  else{
    printf("No command given so nothing done! Refer to readme.\n");
    return;
  }

  
}
