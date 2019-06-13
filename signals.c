#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>

int id = 0;
pid_t zombies[5];
char buf[50];


void sigchather_p(int signum){ // parent signal handler 

    sleep(1);
    printf("PID %d caught one\n",getpid());
  
    for(int i = 1; i < 5; i++ ){ // the parent killing all the zombies

      sprintf(buf, "process %d is dead\n",zombies[i]);
      write(1, buf, strlen(buf));

      kill(zombies[i],9);
    }  
     sprintf(buf,"process %d is dead\n",zombies[0]);
     write(1, buf, strlen(buf));

    _exit(0); 
   

}


void sigchather(int signum){ // children signal handler
     

    printf("PID %d caught one\n",getpid());
       
       for (int i = 4; i > 1; i--){

         if(getpid() == zombies[i]){
            kill(zombies[i-1],2);
         }
       }
       
    if(getpid() == zombies[1]) 
        kill(zombies[0],2);
   


      
    _exit(0);
    
}


int main(){

  pid_t pid; // temp value for fork function answer

  zombies[id]=getpid();

  printf("PID %d ready\n",zombies[id]);
  signal(2 , sigchather_p);


for (int i = 1; i < 5; i++) { // create 4 process in TOTAL 5 (father process include that)
  
  pid = fork();

  if(!pid){    //child

      signal(2 , sigchather);
      printf("PID %d ready\n",getpid());
       zombies[i] = getpid();
      pause();
       
    break;
  }

  zombies[i] = pid;
}



if(getpid() == zombies[0]){
  
    sleep(1);
    kill(zombies[4],2);
    pause();
   
}


}