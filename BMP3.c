#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

#define BMP_HEADER_SIZE 14
#define DIB_HEADER_SIZE 40

 struct BMPInfo{
    char numef[300];
    int height;
    int width;
    long fsize;
    char fuser[50];
    char fmodd[100];
    int fnrleg;
    char ruser[10];
    char rgrup[10];
    char rother[10];

};

typedef struct BMPInfo BMPI;



typedef struct {
    unsigned char magic[2];
    unsigned int  size;
    unsigned int reserved1;
    unsigned int data_offset;
}BMPHeader;
typedef struct {
    unsigned int  headerSize;
    int width;
    int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int imageSize;
    int xPixelsPerMeter;
    int yPixelsPerMeter;
    unsigned int totalColors;
    unsigned int importantColors;
} BMPHeaderInfo;




BMPI getbmp(const char *files){

struct stat fileStat;
BMPI BI={"",0,0,0,"ERROR","",0,"","",""};



int fd = open(files, O_RDONLY);
                int fd1;
                if (fd == -1) {
                    perror("Error opening file");
                    return BI;
                }
    BMPHeader bmp_header;
    BMPHeaderInfo bmp_header_info;
    if (read(fd, &bmp_header, BMP_HEADER_SIZE) != BMP_HEADER_SIZE) {
        perror("Error reading BMP header");
        close(fd);
        return BI;
    }

    if (bmp_header.magic[0] != 'B' || bmp_header.magic[1] != 'M') {
        fprintf(stderr, "Not a valid BMP file.\n");
        close(fd);
        return BI;
    }

    if (read(fd, &bmp_header_info, DIB_HEADER_SIZE ) != DIB_HEADER_SIZE)  {
        perror("Error reading BMP header");
        close(fd);
        return BI;
    }
    
    BI.width = bmp_header_info.width;
    BI.height = bmp_header_info.height;

     if (stat(files, &fileStat) == -1) {
        perror("stat");
        return BI;
    }


    off_t fileSize = fileStat.st_size;
    BI.fsize=fileSize;

    struct passwd *user_id =getpwuid(fileStat.st_uid);
     if (user_id == NULL) {
        perror("getpwuid");
        return BI;
    }

    strcpy(BI.fuser,user_id->pw_name);

    struct tm *latmod=localtime(&fileStat.st_mtime);
        if (latmod == NULL) {
        perror("localtime");
        return BI;
    }

    char time_str[100];
     strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", latmod);
    strcpy(BI.fmodd,time_str);

    nlink_t num_links = fileStat.st_nlink;
    BI.fnrleg=num_links;

    char *t1=(fileStat.st_mode  & S_IRUSR) ? "r" : "-";
    char *t2=(fileStat.st_mode  & S_IWUSR) ? "w" : "-";
    char *t3=(fileStat.st_mode  & S_IXUSR) ? "x" : "-";
    char t[3];
    strcpy(t,t1);
    strcat(t,t2);
    strcat(t,t3);
    strcpy(BI.ruser,t);
    t1=(fileStat.st_mode  & S_IRGRP) ? "r" : "-";
    t2=(fileStat.st_mode  & S_IWGRP) ? "w" : "-";
    t3=(fileStat.st_mode  & S_IXGRP) ? "x" : "-";
    char tt[3];
    strcpy(tt,t1);
    strcat(tt,t2);
    strcat(tt,t3);
    strcpy(BI.rgrup,tt);
    t1=(fileStat.st_mode  & S_IROTH) ? "r" : "-";
    t2=(fileStat.st_mode  & S_IWOTH) ? "w" : "-";
    t3=(fileStat.st_mode  & S_IXOTH) ? "x" : "-";
    char ttt[3];
    strcpy(ttt,t1);
    strcat(ttt,t2);
    strcat(ttt,t3);
    strcpy(BI.rother,ttt);


      close(fd);
return BI;
}


BMPI getfile(const char *files){

struct stat fileStat;
BMPI BI={"",0,0,0,"ERROR","",0,"","",""};



int fd = open(files, O_RDONLY);
                int fd1;
                if (fd == -1) {
                    perror("Error opening file");
                    return BI;
                }
    

     if (stat(files, &fileStat) == -1) {
        perror("stat");
        return BI;
    }


    off_t fileSize = fileStat.st_size;
    BI.fsize=fileSize;

    struct passwd *user_id =getpwuid(fileStat.st_uid);
     if (user_id == NULL) {
        perror("getpwuid");
        return BI;
    }

    strcpy(BI.fuser,user_id->pw_name);

    struct tm *latmod=localtime(&fileStat.st_mtime);
        if (latmod == NULL) {
        perror("localtime");
        return BI;
    }

    char time_str[100];
     strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", latmod);
    strcpy(BI.fmodd,time_str);

    nlink_t num_links = fileStat.st_nlink;
    BI.fnrleg=num_links;

    char *t1=(fileStat.st_mode  & S_IRUSR) ? "r" : "-";
    char *t2=(fileStat.st_mode  & S_IWUSR) ? "w" : "-";
    char *t3=(fileStat.st_mode  & S_IXUSR) ? "x" : "-";
    char t[3];
    strcpy(t,t1);
    strcat(t,t2);
    strcat(t,t3);
    strcpy(BI.ruser,t);
    t1=(fileStat.st_mode  & S_IRGRP) ? "r" : "-";
    t2=(fileStat.st_mode  & S_IWGRP) ? "w" : "-";
    t3=(fileStat.st_mode  & S_IXGRP) ? "x" : "-";
    char tt[3];
    strcpy(tt,t1);
    strcat(tt,t2);
    strcat(tt,t3);
    strcpy(BI.rgrup,tt);
    t1=(fileStat.st_mode  & S_IROTH) ? "r" : "-";
    t2=(fileStat.st_mode  & S_IWOTH) ? "w" : "-";
    t3=(fileStat.st_mode  & S_IXOTH) ? "x" : "-";
    char ttt[3];
    strcpy(ttt,t1);
    strcat(ttt,t2);
    strcat(ttt,t3);
    strcpy(BI.rother,ttt);


      close(fd);
return BI;
}



int writebmp(BMPI BIS){

int fd1;
fd1=open("statistica.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume fisier: %s\n", BIS.numef);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "inaltime: %d\n", BIS.height);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "lungime: %d\n", BIS.width);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "timpul ultimei modificari: %s\n", BIS.fmodd);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "contorul de legaturi: %lu\n", (unsigned long)BIS.fnrleg);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    write(fd1, output_buffer, strlen(output_buffer));
 
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    write(fd1, output_buffer, strlen(output_buffer));



    close(fd1);

    return 1;

}

int writefile(BMPI BIS){

int fd1;
fd1=open("statistica.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume fisier: %s\n", BIS.numef);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
    write(fd1, output_buffer, strlen(output_buffer));


    sprintf(output_buffer, "timpul ultimei modificari: %s\n", BIS.fmodd);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "contorul de legaturi: %lu\n", (unsigned long)BIS.fnrleg);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    write(fd1, output_buffer, strlen(output_buffer));
 
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    write(fd1, output_buffer, strlen(output_buffer));



    close(fd1);

    return 1;

}

int writelink(BMPI BIS){

int fd1;
fd1=open("statistica.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume legatura: %s\n", BIS.numef);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    write(fd1, output_buffer, strlen(output_buffer));

    sprintf(output_buffer, "fisier target: %s\n", BIS.fmodd);
    write(fd1, output_buffer, strlen(output_buffer));

    
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    write(fd1, output_buffer, strlen(output_buffer));
 
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    write(fd1, output_buffer, strlen(output_buffer));



    close(fd1);

    return 1;

}

int writedir(BMPI BIS){

int fd1;
fd1=open("statistica.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume director: %s\n", BIS.numef);
    write(fd1, output_buffer, strlen(output_buffer));

     sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
    write(fd1, output_buffer, strlen(output_buffer));

   
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    write(fd1, output_buffer, strlen(output_buffer));
 
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    write(fd1, output_buffer, strlen(output_buffer));
    
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    write(fd1, output_buffer, strlen(output_buffer));



    close(fd1);

    return 1;

}


int  main(int argc, char *argv[]) {
 pid_t pid1, pid2;
 int status;
 BMPI BI1;
 BMPI BI2={"",0,0,0,"NEW","",0,"","",""};
   
 if (argc != 3) {
      //  printf("Usage: %s <directory_path> \n", argv[0]);
        return 1;
    }
    DIR *dir, *dir1;
    const char *path = argv[1];
    const char *path1 = argv[2];
    struct dirent *entry;
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return 0;
    }

     while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

    
        if (stat(fullpath, &statbuf) == -1) {
            perror("Error getting file information");
            continue;
        }

          

    
         if (S_ISDIR(statbuf.st_mode)) {
            printf("dir: %s\n", entry->d_name);
            strcpy(BI2.numef, entry->d_name);
          
            struct passwd *user_id =getpwuid(statbuf.st_uid);
            if (user_id == NULL) {
                perror("getpwuid");
              }

         strcpy(BI2.fuser,user_id->pw_name);


           // printf("user: %s\n", BI1.fuser);

            char *t1=(statbuf.st_mode  & S_IRUSR) ? "r" : "-";
            char *t2=(statbuf.st_mode  & S_IWUSR) ? "w" : "-";
            char *t3=(statbuf.st_mode  & S_IXUSR) ? "x" : "-";
            char t[3];
            strcpy(t,t1);
            strcat(t,t2);
            strcat(t,t3);
            strcpy(BI2.ruser,t);
            t1=(statbuf.st_mode  & S_IRGRP) ? "r" : "-";
            t2=(statbuf.st_mode  & S_IWGRP) ? "w" : "-";
            t3=(statbuf.st_mode  & S_IXGRP) ? "x" : "-";
            char tt[3];
            strcpy(tt,t1);
            strcat(tt,t2);
            strcat(tt,t3);
            strcpy(BI2.rgrup,tt);
            t1=(statbuf.st_mode  & S_IROTH) ? "r" : "-";
            t2=(statbuf.st_mode  & S_IWOTH) ? "w" : "-";
            t3=(statbuf.st_mode  & S_IXOTH) ? "x" : "-";
            char ttt[3];
            strcpy(ttt,t1);
            strcat(ttt,t2);
            strcat(ttt,t3);
            strcpy(BI2.rother,ttt);
            pid1=fork();
            if(pid1<0){
                perror("Eroare fork");
               exit(EXIT_FAILURE);
            } else if(pid1==0){
            writedir(BI2);
            printf("Pid copil %d, pid parinte %d\n", getpid(), getppid());
            } else {
    waitpid(pid1, &status,0);
    if (WIFEXITED(status)) {
                printf("Copil PID=%d terminat cu  status %d\n", pid1, WEXITSTATUS(status));
            }
     printf("Parinte PID %d  parinte PID %d\n", getpid(), getppid());
    }
           

        } else if (S_ISREG(statbuf.st_mode)) {
          
                         
            const char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL && strcmp(extension, ".bmp") == 0) {
              //  printf("file bmp: %s\n", entry->d_name);
                const char *files=entry->d_name;
              BI1=getbmp(files);
            //    printf("Image Width: %d\n", BI1.width);
           //     printf("Image Height: %d\n", BI1.height);
                strcpy(BI1.numef,entry->d_name);
               // printf("nume fisier: %s\n",BI1.numef);
               // printf("data: %s\n",BI1.fmodd);
              //  printf("pu: %s\n",BI1.ruser);
                writebmp(BI1);

            } else{
                  //  printf("(statbuf %d\n", entry->d_type);
                if(entry->d_type==10){
                    const char *files=entry->d_name;
                    BI1=getfile(files);
                    strcpy(BI1.numef,entry->d_name);
                    char buf1[100];
                    ssize_t len1 = readlink(fullpath, buf1, sizeof(buf1));
                   // printf("ffff %s\n", buf1);
                    strcpy(BI1.fmodd,buf1);
                    writelink(BI1);

                }else
                {
               
                 const char *files=entry->d_name;
              
                BI1=getfile(files);
                strcpy(BI1.numef,entry->d_name);
                writefile(BI1);
                }
            }
            }
             
                       
        }
         
        }
     
