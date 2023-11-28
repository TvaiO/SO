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

 typedef struct BMPInfo{
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
    char dir2[100];
} BMPI;

//typedef struct BMPInfo BMPI;

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

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} BMPPixel;

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

int writefile(BMPI BIS){

int fd1, ii=0,jj,k=0, in=0, staus;
fd1=open(strcat(BIS.dir2,"/fisier_statistica.txt"), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        k=-1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume fisier: %s\n", BIS.numef);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;

    sprintf(output_buffer, "timpul ultimei modificari: %s\n", BIS.fmodd);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "contorul de legaturi: %lu\n", (unsigned long)BIS.fnrleg);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    jj=write(fd1, output_buffer, strlen(output_buffer));
    if(jj == -1) {
        perror("Error opening file");
       k=-2;
    } else ii++;


    close(fd1);
    if(k!=0){
        return k;
    }
    else return ii;

}

int writelink(BMPI BIS){

int fd1, jj, ii=0, k;
fd1=open(strcat(BIS.dir2,"/fisierlink_statistica.txt"), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume legatura: %s\n", BIS.numef);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    sprintf(output_buffer, "fisier target: %s\n", BIS.fmodd);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++; 


    close(fd1);

   if(k!=0){
        return k;
    }
    else return ii;

}

int modifybmp(const char *filename){
   int i,j;
BMPHeader header;
BMPHeaderInfo hi;
struct stat fileStat;

  printf("Color modification started.\n");

   // const char *filename = "imagine1.bmp";
    // const char *filename1= "imagine11.bmp";
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

     if (read(fd, &header, BMP_HEADER_SIZE) != BMP_HEADER_SIZE) {
        perror("Error reading BMP header");
        close(fd);
        return -1;
    }

    if (header.magic[0] != 'B' || header.magic[1] != 'M') {
        fprintf(stderr, "Not a valid BMP file.\n");
        close(fd);
        return -2;
    }

    if (read(fd, &hi, DIB_HEADER_SIZE ) != DIB_HEADER_SIZE)  {
        perror("Error reading BMP header");
        close(fd);
        return -3;
    }
    
 if (stat(filename, &fileStat) == -1) {
        perror("stat");
        return -4;
    }

 printf("Color in process.\n");
//read(fd, &header, sizeof(BMPHeader)); 
//read(fd, &hi, sizeof(BMPHeaderInfo)); 
int pgri;
lseek(fd,sizeof(BMPHeader)+ sizeof(BMPHeaderInfo),SEEK_SET);
 unsigned char ppixel[3];

  while (read(fd, ppixel, sizeof(ppixel)) > 0) {
        pgri= 0.299*ppixel[2]+0.587*ppixel[1]+0.114*ppixel[0];
       
       
       ppixel[0] = pgri;
        ppixel[1] = pgri;
        ppixel[2] = pgri;

        lseek(fd, -sizeof(ppixel), SEEK_CUR);
        if (write(fd, ppixel, sizeof(ppixel)) == -1) {
            perror("Error writing modified pixel");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    // Close the file
    close(fd);

    printf("Color modification complete.\n");

}

BMPI getbmp(const char *files){

struct stat fileStat;
BMPI BI={"",0,0,0,"ERROR","",0,"","",""};



int fd = open(files, O_RDONLY);
               // int fd1;
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

int writebmp(BMPI BIS){


int fd1, ii=0,jj,k=0;
fd1=open(strcat(BIS.dir2,"/fisierBMP_statistica.txt"), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return 1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume fisier: %s\n", BIS.numef);
   jj= write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "inaltime: %d\n", BIS.height);
   jj=  write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "lungime: %d\n", BIS.width);
    jj= write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "dimensiune: %ld\n", (long)BIS.fsize);
    jj= write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
   jj=  write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "timpul ultimei modificari: %s\n", BIS.fmodd);
   jj=  write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
    sprintf(output_buffer, "contorul de legaturi: %lu\n", (unsigned long)BIS.fnrleg);
    jj= write(fd1, output_buffer, strlen(output_buffer));
  if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;   
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
   jj=  write(fd1, output_buffer, strlen(output_buffer));
  if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
  jj=   write(fd1, output_buffer, strlen(output_buffer));
  if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;   
    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    jj= write(fd1, output_buffer, strlen(output_buffer));
 if(jj == -1) {
        perror("Error opening file");
        k=-2;
    } else ii++;


    close(fd1);

    if(k!=0){
        return k;
    }
    else return ii;

}



int writedir(BMPI BIS){

int fd1, ii, jj;
ii=0;
fd1=open(strcat(BIS.dir2,"/director_statistica.txt"), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
   if(fd1 == -1) {
        perror("Error opening file");
        return -1;
    }
    
    char output_buffer[1024];

    sprintf(output_buffer, "nume director: %s\n", BIS.numef);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        return -2;
    } else ii++;
    

     sprintf(output_buffer, "identificatorul utilizatorului: %s\n", BIS.fuser);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        return -2;
    } else ii++;
   
    sprintf(output_buffer, "drepturi de acces user:  %s\n", BIS.ruser);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        return -2;
    } else ii++;
 
    sprintf(output_buffer, "drepturi de acces grup:  %s\n", BIS.rgrup);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        return -2;
    } else ii++;

    sprintf(output_buffer, "drepturi de acces altii: %s\n", BIS.rother);
    jj=write(fd1, output_buffer, strlen(output_buffer));
     if(jj == -1) {
        perror("Error opening file");
        return -2;
    } else ii++;


    close(fd1);

    return ii;

}

int  main(int argc, char *argv[]) {

if (argc != 3) {
        printf("Usage: %s <directorysource_path> <directory_destination>\n", argv[0]);
        return 1;
    }
pid_t pid, pid1, pid2, pid3, pid4;
    int status, status1, status2, status3, status4, status5, in=0;
    int i,j, d1, d2, jj1,d3,d4;
DIR *dir, *dir1;
    const char *path = argv[1];
    const char *path1=argv[2];
    struct dirent *entry;
    struct stat statbuf;

 BMPI BI1;
 BMPI BI2={"",0,0,0,"NEW","",0,"","",""};

    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return 0;
    }

     dir1 = opendir(path1);
    if (dir1 == NULL) {
        perror("Error opening directory");
        return 0;
    }

    int fd;
    char output_buffer1[1024];
char numeobj[300];
//fd=open("statistica.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR );
//   if(fd == -1) {
//        perror("Error opening file");
     
//    }
    
    char output_buffer[1024];


     while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

    
        if (lstat(fullpath, &statbuf) == -1) {
            perror("Error getting file information");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("dir: %s\n", entry->d_name);
            strcpy(numeobj, entry->d_name);
            pid=fork();
            if(pid<0){
                perror("Eroare fork");
                exit(EXIT_FAILURE);
            } else if(pid==0){
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
            lseek(fd,sizeof(BMPHeader),0);
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
            strcpy(BI2.dir2,"DIR2");
            d1=writedir(BI2);

         exit(d1);
    } 
    }


       // printf("%d\n", statbuf.st_mode);
        if (S_ISLNK(statbuf.st_mode)) {
               
            printf("link: %s\n", entry->d_name);
            strcpy(numeobj, entry->d_name);

            pid4=fork();
            if(pid4<0){
                perror("Eroare fork");
                exit(EXIT_FAILURE);
            } else if(pid4==0){
                const char *files=entry->d_name;
                    BI1=getfile(files);
                    strcpy(BI1.numef,entry->d_name);
                    char buf1[100];
                    ssize_t len1 = readlink(fullpath, buf1, sizeof(buf1));
                    printf("ffff %s\n", buf1);
                    strcpy(BI1.fmodd,buf1);
                    strcpy(BI1.dir2,"DIR2");
                    d2=writelink(BI1);

                        exit(d2);
                } 
        
        }

        if (S_ISREG(statbuf.st_mode)){
            printf("file: %s\n", entry->d_name);
            strcpy(numeobj, entry->d_name);

             const char *extension = strrchr(entry->d_name, '.');
            if (extension != NULL && strcmp(extension, ".bmp") == 0) {

            pid2=fork();
            if(pid2<0){
                perror("Eroare fork");
                exit(EXIT_FAILURE);
            } else if(pid2==0){

           
                printf("file bmp conver to gray: %s\n", entry->d_name);
            
                const char *files=entry->d_name;
              BI1=getbmp(files);

                strcpy(BI1.numef,entry->d_name);
                 strcpy(BI1.dir2,"DIR2");
                //d2=writefile(BI1);

                d3=writebmp(BI1);
             exit(d3);
        } 
         pid3=fork();
            if(pid3<0){
                perror("Eroare fork");
                exit(EXIT_FAILURE);
            } else if(pid3==0){
                jj1=modifybmp(numeobj);

                exit(0);
            } 

 //}
    //convert bmp

           } 
        
        
    
        else {
        strcpy(numeobj, entry->d_name);
        pid1=fork();
    if(pid1<0){
        perror("Eroare fork");
        exit(EXIT_FAILURE);
    } else if(pid1==0){
          const char *files=entry->d_name;
              
                BI1=getfile(files);
                strcpy(BI1.numef,entry->d_name);
                 strcpy(BI1.dir2,"DIR2");
                d4=writefile(BI1);
                exit(d4);
        } 
        }

}
in++;
     }
for (i=0; i<in-1; i++)
{
printf("id %d\n", i);
int wpid = wait(&status);
if(WIFEXITED(status))
printf("S-a încheiat procesul cu pid-ul %d si codul  %d \n", wpid, WEXITSTATUS(status));
else
printf("\nChild %d ended abnormally\n", wpid);
}
}