#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/wait.h>


#include "md5.h"
 
using namespace std;
char send_buffer[33];
string hashed="hello";
int is_ok;

void hash_check(int sig_num){ //checking the hash length
    int count = 0;
    for (int i = 0; i < 33; i++){ 
        if(send_buffer[i]!='\0')
        count++;
    }
    
     if (count == 32)
         is_ok = 1;
     else is_ok = 0;



}


int main(int argc, char *argv[])
{
    int pipe_fd[2]; // file descriptor
    char input_string[21];
    



 
    
    if(pipe(pipe_fd) == 0){ // opening the pipes
        
        pid_t pid = fork(); 
        
        if(pid < 0){ cerr << "fork error" << endl; exit(1);}

         
        if(pid){ // parent

            cout << "plain text: ";
            fgets(input_string ,21 ,stdin); // getting the string from user

            //cout << "PID parent :" << getpid() << endl;
            //cout << "pid child :" << pid << endl;
            
            int count_w = write(pipe_fd[1] ,input_string ,strlen(input_string)); // sending the string to the child process
            sleep(1); 
            int count_r = read(pipe_fd[0] , send_buffer , 33); // reciving the hashed string 
            signal(2,hash_check);

            //closeing the pipes
            close(pipe_fd[0]); 
            close(pipe_fd[1]);
            pause();

            if(is_ok)
                cout << "encrypted by process " << pid << " : " << send_buffer << endl;
                
            else 
                cout << "error in hashing " << endl;

            
            kill(pid,9); // killing the child 
            
        }
        else { // child

            int count_r = read(pipe_fd[0] , send_buffer, 21); // reading string from the parrent 

            hashed = md5(send_buffer); // the hashing function

            int count_w = write(pipe_fd[1] ,hashed.c_str() ,33); // sending the hashed string 
            
            //closeing the pipes
            close(pipe_fd[0]); 
            close(pipe_fd[1]);
          
           
           sleep(2);
           kill(getppid(),2); // sending a signal to the parent to start hash_check
           
           pause(); // pauseing the exeution of the child process


        }



    }

    _exit(0);
}