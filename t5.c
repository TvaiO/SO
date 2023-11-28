#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int pipe1[2], pipe2[2], pipe3[2];
    pid_t pid1, pid2;
    int status;

    if(pipe(pipe1)==-1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) {
        perror("Error creating pipe");
    }


    pid1=fork();
    if(pid1<0){
        perror("Error creating first child");
    } else if (pid1 == 0) { 
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe3[0]);
            close(pipe3[1]);
       char ch;
       while(read(pipe1[0], &ch, sizeof(char))>0){
            if ('a' <= ch && ch <= 'z') {
                write(pipe2[1], &ch, sizeof(char));
            }
       }
       close(pipe1[0]); 
       close(pipe2[1]);
       exit(EXIT_SUCCESS);
    } else {

         if ((pid2 = fork()) == -1) {
            error("Error creating second child");
        } else if (pid2 == 0) { // Second child code
            close(pipe1[1]);
            close(pipe1[0]);
            close(pipe2[1]);
            close(pipe3[0]);
       

            int statFile = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
            if (statFile == -1) {
                error("Error creating statistica.txt");
            }

            char letter;
            int count[26] = {0};
            int j=0;
            while (read(pipe2[0], &letter, sizeof(char)) > 0) {
                if ('a' <= letter && letter <= 'z') {
                    count[letter - 'a']++;
                    
                }
            }

            for (int i = 0; i < 26; i++) {
                if (count[i] > 0) {
                    dprintf(statFile, "%c: %d\n", 'a' + i, count[i]);
                    j++;
                }
  
           }
     //   write(pipe3[1], &j, sizeof(int));
        printf("Nr linii %d\n", j);

        close(statFile);
        close(pipe2[0]);
        close(pipe3[1]);
        } else{  
        close(pipe1[0]);
         close(pipe2[0]);
          close(pipe2[1]);
          close(pipe3[1]);

       


        int file = open("date.txt", O_RDONLY);
       
        if(file==-1){
            perror("Error opening file");
        }

        char ch;
        while(read(file, &ch, sizeof(char))>0){
            write(pipe1[1], &ch,sizeof(char));
        }


        int k=0;
      //  read(pipe3[0], &k, sizeof(int));
        printf("Number of distinct letters: %d\n", k);

        

        close(file);
        close(pipe1[1]);
        close(pipe3[0]);
      
          waitpid(pid2, NULL, 0);
        
          waitpid(pid1, NULL, 0);

      int wpid = wait(&status);
if(WIFEXITED(status))
printf("\nChild %d ended with code %d\n", wpid, WEXITSTATUS(status));
    }

}
}