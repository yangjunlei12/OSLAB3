#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h> 
#include <fcntl.h> 
#include <sys/wait.h>
#define PIPLEN 100
sem_t sem;


int pipeSize(){
  int ret,count=0,filedes[2];  
  pipe(filedes);
    fcntl(filedes[1],F_SETFL,O_NONBLOCK); 
    while(1)  
    {  
        ret=write(filedes[1],"1",1);
        if(ret==-1)  break;   
        count++;  
    }  
    printf("The length of unamed pipe is:%dB\n\n",count);
    close(filedes[0]);
    close(filedes[1]);
    return count;
}

void err(){
    printf("fork error!\n"); 
    exit(0);
}

int main()
{
   int fd[2],p1,p2,p3,len,actLen;      
   char outpipe[PIPLEN],inpipe[PIPLEN];
   int maxSize=pipeSize();
   pipe(fd); //create a pipe                    
   sem_init(&sem,1,1);
   if((p1=fork())==-1){
      err();
    }       
   if(p1==0)
   {
      sem_wait(&sem);
      close(fd[0]);     
      sprintf(outpipe,"This is test for pid1\n");         
      len=strlen(outpipe);
      printf("pid1 will send %d bytes\n",len);
      actLen=write(fd[1],outpipe,len);  //actually write in
      printf("pid1 have sent %d bytes\n",actLen);
      sleep(1);       
      sem_post(&sem);
      exit(0);               
   }
   else
   {
       if((p2=fork())==-1){
            err();
       }
       if(p2==0)
       {
          sem_wait(&sem);
          close(fd[0]);       
          sprintf(outpipe,"This is test for pid2\n");         
          len=strlen(outpipe);
          printf("pid2 will send %d bytes\n",len);
          actLen=write(fd[1],outpipe,len);  
          printf("pid2 have sent %d bytes\n",actLen);
          sleep(1);
          sem_post(&sem);
          exit(0);     
       }
        else
        {
           if((p3=fork())==-1){
              err();
           }
           if(p3==0)
           {
              sem_wait(&sem);
              close(fd[0]);       
              sprintf(outpipe,"This is test for pid3\n");         
              len=strlen(outpipe);
              printf("pid3 will send %d bytes\n",len);
              actLen=write(fd[1],outpipe,len);  
              printf("pid3 have sent %d bytes\n",actLen);
              sleep(1);
              sem_post(&sem);
              exit(0);     
           }
            else
           {
              wait(0);           
              read(fd[0],inpipe,PIPLEN);
              printf("%s\n",inpipe);
              wait(0);           
              read(fd[0],inpipe,PIPLEN);
              printf("%s\n",inpipe);
              wait(0);         	  
              read(fd[0],inpipe,PIPLEN);
              printf("%s\n",inpipe);
              exit(0);
           }
         }
     }
   return 0;
}