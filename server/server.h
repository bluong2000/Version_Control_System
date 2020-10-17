#ifndef SERVER_H 
#define SERVER_H 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h> 
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <openssl/opensslconf.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/opensslv.h>
#include <time.h>
#include <signal.h>


void delay(int number_of_seconds) // change eventually
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 


int isNumber(char* str) {

    int c = 0;
    while (c < strlen(str)) {
        if (isdigit(str[c]) == 0) {
            return -1;
        }
        c++;
    }
    return 1;
}


char* readSock(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 40)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 40 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: client unable to send data\n");
  //      ////close(sockfd);
        return NULL; //exit(-1);
    }

    char* buff = (char*) malloc((arrlen + 1) * sizeof(char));
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                //////close(sockfd);
                return NULL; //exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

    return buff;
}



char* fillBuff (char* file) {

    int mc = 0;

    int fTemp = open(file, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);

    //printf("File size is %d\n\n", (int)fsize);

    char *buff = malloc(fsize*sizeof(char) + 1); 
    while (buff == NULL) {

        if (mc == 2) {

            fprintf(stderr, "Fatal Error: malloc was unable to allocate memory after %d tries", mc);
            file = NULL;
            return NULL;
        }
        fprintf(stderr, "Error: malloc failed to allocate memory, trying again");
        buff = malloc(fsize*sizeof(char) + 1);
        mc++;
    }
    mc = 0;

    buff[fsize] = '\0';
    int readIn = 0;
    int status = 0;
    int fd = open(file, O_RDONLY);


    do{  // reads buffer char by char
            status = read(fd, buff+readIn, fsize - readIn);
            readIn += status;
                
                    
    } while (status > 0 && readIn < fsize); 

    close(fd);

    return buff;
}




char* readSockwithLen(int sockfd, int arrlen) {

    char* buff = (char*) malloc((arrlen + 1) * sizeof(char));
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                //////close(sockfd);
                return NULL; //exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

    return buff;
}



int findBytes(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 400)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 400 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: client unable to send data\n");
        //////close(sockfd);
        return -1; //exit(-1);
    }

    return arrlen;

}





char* fillTar (char* file) {

    int mc = 0;

    int fTemp = open(file, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);

    char *buff = malloc(fsize*sizeof(char)); 
    while (buff == NULL) {

        if (mc == 2) {

            fprintf(stderr, "Fatal Error: malloc was unable to allocate memory after %d tries", mc);
            file = NULL;
            return NULL;
        }
        fprintf(stderr, "Error: malloc failed to allocate memory, trying again");
        buff = malloc(fsize*sizeof(char));
        mc++;
    }
    mc = 0;

    int readIn = 0;
    int status = 0;
    int fd = open(file, O_RDONLY);


    do{  // reads buffer char by char
            status = read(fd, buff+readIn, fsize - readIn);
            readIn += status;
                
                    
    } while (status > 0 && readIn < fsize); 

    close(fd);

    return buff;
}


void recPrac(char* f, int (*fun)(const char*)) { // dont parse '.' and '..' when recursing through the directories

    //printf("\nSTART RECURSION WITH: \"%s\"\n", f);

    DIR* currentDir = opendir(f);

    struct dirent* currentThing = NULL;
    readdir(currentDir);
    readdir(currentDir);
    //currentThing = readdir(currentDir);

    while (currentThing = readdir(currentDir)) {

        if ( currentThing->d_type == DT_REG ) {

            

            char* temp = (char*)calloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2, 1);
            strcpy(temp, f);
            strcat(temp, "/");
            strcat(temp, currentThing->d_name);

            //printf("\nFile: %s\n", temp);
            int x = fun(temp);
            if (x != 0)
                printf("\tFile not removed\n");
            free(temp);
            
        }
        else if ( currentThing->d_type == DT_DIR ) { 

            //printf("Dir: %s\n", currentThing->d_name);
            char* temp = (char*)malloc((strlen(f) + strlen(currentThing->d_name)) * sizeof(char) + 2);
            strcpy(temp, f);
            strcat(temp, "/"); //printf("\n\n%s%s\n\n", temp, currentThing->d_name);

            recPrac(strcat(temp, currentThing->d_name), fun); 
            free(temp);
        }
        
    } 
    closedir(currentDir);
    printf("\nDir: %s\n", f);
    int x = fun(f);
        if (x != 0)
            printf("Dir not removed\n");

    return;
}



int checkout(int sockfd, int newsockfd, char* proj) {

    char fullP[strlen("./repository/") + strlen(proj) + 1];
    fullP[strlen("./repository/") + strlen(proj)] = '\0';
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in == NULL) {

        int n = write(newsockfd, "Error: project does not exist in server repository", strlen("Error: project does not exist in server repository"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            //////close(sockfd);
            close(newsockfd);
        }
        return -1;
    }
    closedir(in);

    char tar[strlen(fullP) + strlen(".tar.gz") + 1];
    tar[strlen(fullP) + strlen(".tar.gz")] = '\0';
    strcpy(tar, fullP);
    strcat(tar, ".tar.gz");

    char command[strlen(" -C ./repository ./") + strlen(tar) + strlen(proj) + strlen("tar -czf ") + 1];
    command[strlen(" -C ./repository ./") + strlen(tar) + strlen(proj) + strlen("tar -czf ")] = '\0';
    strcpy(command, "tar -czf ");
    strcat(command, tar);
    strcat(command, " -C ./repository ./");
    strcat(command, proj);

    //printf("Command: \"%s\"", command);

    if (system(command) == -1) {

        fprintf(stderr, "Fatal Error: unable to archive\n");
        //////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }

    char* bin = fillTar(tar); 

    int fTemp = open(tar, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);

    int n = write(newsockfd, bin, (int)fsize);
    if (n < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        //////close(sockfd);
        close(newsockfd);
    }


    remove(tar);
    return 1;
}


int update(int sockfd, int newsockfd, char* proj) {
  
   
    //creates file path to project
    char fullP[strlen("./repository/") + strlen(proj) + 1];
    fullP[strlen("./repository/") + strlen(proj)] = '\0';
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in == NULL) {

        int n = write(newsockfd, "Error: project does not exist in server repository", strlen("Error: project does not exist in server repository"));

        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            //////close(sockfd);
            close(newsockfd);
        }
        return -1;
    } else {  //project exists

        //create manifest path
        char manPath[strlen(fullP)  + strlen("/.Manifest") + 1]; //done
        manPath[strlen(fullP) +  strlen("/.Manifest")] = '\0'; //done
        strcpy(manPath, fullP); 
        strcat(manPath, "/");         
     //   strcat(manPath, proj); 
        strcat(manPath, ".Manifest"); 

     //   printf("%s", manPath);
        //opens manifest and fills it into a buffer
        int fd = open(manPath, O_RDWR);
        if (fd < 0) {

            int n = write(newsockfd, "Error: unable to open server manifest file", strlen("Error: unable to open server manifest file"));
            if (n < 0) {

                fprintf(stderr, "Fatal Error: unable to write to socket\n");
            }
            //////close(sockfd);
            close(newsockfd);
            //return -1; //exit(-1);
            return -1;
        }
        close(fd);

        char* man = fillBuff(manPath);

        //sends manifest buffer to socket to client side
        int n = write(newsockfd, man, strlen(man));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }

        //printfs ack of manifest and project

        /*
        int n = write(newsockfd, "Project exists!", strlen("Project exists!"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            //////close(sockfd);
            close(newsockfd);
        }
        */
     //   return -1;



    }
    closedir(in);

    return 1;
    

}



int upgrade(int sockfd, int newsockfd, char* proj) {

    char projPath[strlen("repository/") + strlen(proj) + 1];
    projPath[strlen("repository/") + strlen(proj)] = '\0';
    strcpy(projPath, "repository/");
    strcat(projPath, proj);

    DIR* _pPath = opendir(projPath);

    if (_pPath == NULL) {

        write(newsockfd, "Error: project does not exist in server repository\n", strlen("Error: project does not exist in server repository\n"));
        close(newsockfd);
        return -1; //exit(-1);
    }
    closedir(_pPath);


    char mani[strlen(projPath) + strlen("/") + strlen(".Manifest") + 1]; //done
    mani[strlen(projPath) + strlen("/") +  strlen(".Manifest")] = '\0'; //done
    strcpy(mani, projPath);
    strcat(mani, "/");
  //  strcpy(mani, proj);
    strcat(mani, ".Manifest");


    char* sMan = fillBuff(mani);


    if (write(newsockfd, sMan, strlen(sMan)) < 0 ) {

        close(newsockfd);
        return -1; //exit(-1);
    }


    char* upBuff = readSock(newsockfd);

    char tarFile[strlen("repository/") + strlen(proj) + strlen("/") + strlen(proj) + 1];
    tarFile[strlen("repository/") + strlen(proj) + strlen("/") + strlen(proj)] = '\0';
    strcpy(tarFile, "repository/");
    strcat(tarFile, proj);
    strcat(tarFile, "/");
    strcat(tarFile, proj);

    mkdir(tarFile, 00700);


    char* curr = strtok(upBuff, "\n");
    curr = strtok(NULL, "\n");



    while(curr != NULL) {

        char* p = strsep(&curr, "\t");
        char* flg = strsep(&curr, "\t");

        char* in = strstr(sMan, p);

    

        if (in == NULL && (strcmp(flg, "A") == 0 || strcmp(flg, "M") == 0) ) {

            write(newsockfd, "Error: update file is out of date/expired, update again in order to upgrade\n", strlen("Error: update file is out of date/expired, update again in order to upgrade\n"));
            free(sMan);
            close(newsockfd);
            return -1; //exit(-1);
        }
        else if (in != NULL && strcmp(flg, "R") == 0) {

            write(newsockfd, "Error: update file is out of date/expired, update again in order to upgrade\n", strlen("Error: update file is out of date/expired, update again in order to upgrade\n"));
            free(sMan);
            close(newsockfd);
            return -1; //exit(-1);
        }
        
        if (strcmp(flg, "A") == 0 || strcmp(flg, "M") == 0) {

            char* lastS = strrchr(p, '/');
            lastS[0] = '\0';
            char justPath[strlen(p) + 1];
            justPath[strlen(p)] = '\0';
            strcpy(justPath, p);
            lastS[0] = '/';

            char repoPath[strlen("repository/") + strlen(proj) + strlen("/") + strlen(justPath) + 1];
            repoPath[strlen("repository/") + strlen(proj) + strlen("/") + strlen(justPath)] = '\0';
            strcpy(repoPath, "repository/");
            strcat(repoPath, proj);
            strcat(repoPath, "/");
            strcat(repoPath, justPath);

            char actualFPath[strlen("repository/") + strlen(p) + 1];
            actualFPath[strlen("repository/") + strlen(p)] = '\0';
            strcpy(actualFPath, "repository/");
            strcat(actualFPath, p);
            

            char moveF[strlen("mkdir -p ") + strlen(repoPath) + strlen(" && cp ") + strlen(actualFPath) + strlen(" ") + strlen(repoPath) + 1];
            moveF[strlen("mkdir -p ") + strlen(repoPath) + strlen(" && cp ") + strlen(actualFPath) + strlen(" ") + strlen(repoPath)] = '\0';
            strcpy(moveF, "mkdir -p ");
            strcat(moveF, repoPath);
            strcat(moveF, " && cp ");
            strcat(moveF, actualFPath);
            strcat(moveF, " ");
            strcat(moveF, repoPath);


            if (system(moveF) < 0) {

                write(newsockfd, "Error: unable to copy files or create subdirectories in temp file\n", strlen("Error: unable to copy files or create subdirectories in temp file\n"));
                close(newsockfd);
                free(sMan);
                recPrac(tarFile, remove);
                return -1; //exit(-1);
            }


        }
        

        curr = strtok(NULL, "\n");
    }


    char compressF[strlen("tar czf temp.tar.gz -C ./repository/") + strlen(proj) + strlen("/") + strlen(proj) + strlen(" .") + 1];
    compressF[strlen("tar czf temp.tar.gz -C ./repository/") + strlen(proj) + strlen("/") + strlen(proj) + strlen(" .")] = '\0';
    strcpy(compressF, "tar czf temp.tar.gz -C ./repository/");
    strcat(compressF, proj);
    strcat(compressF, "/");
    strcat(compressF, proj);
    strcat(compressF, " .");



    if (system(compressF) < 0) {

        write(newsockfd, "Error: unable to copy files or create subdirectories in temp file\n", strlen("Error: unable to copy files or create subdirectories in temp file\n"));
        close(newsockfd);
        free(sMan);
        recPrac(tarFile, remove);
        return -1; //exit(-1);
    }


    char* sendTar = fillTar("temp.tar.gz");

    int fTemp = open("temp.tar.gz", O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);
    

    write(newsockfd, sendTar, (int)fsize);



    free(sMan);
    recPrac(tarFile, remove);
    remove("temp.tar.gz");

    return 1;
}


int commit(int sockfd, int newsockfd, char* proj) {

    char fullP[strlen("./repository/") + strlen(proj) + 1];
    fullP[strlen("./repository/") + strlen(proj)] = '\0';
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in == NULL) {

        int n = write(newsockfd, "Error: project does not exist in server repository", strlen("Error: project does not exist in server repository"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            //////close(sockfd);
            close(newsockfd);
        }
        return -1;
    }
    closedir(in);

    char manifest[strlen(fullP)  + strlen(".Manifest/") + 1]; //done
    manifest[strlen(fullP)  + strlen(".Manifest/")] = '\0'; //done
    strcpy(manifest, fullP);
    strcat(manifest, "/");
 //   strcat(manifest, proj);
    strcat(manifest, ".Manifest");

    int fd = open(manifest, O_RDWR);
    if (fd < 0) {

        int n = write(newsockfd, "Error: unable to open server manifest file", strlen("Error: unable to open server manifest file"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        //////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    //added close
    close(fd);

    char* man = fillBuff(manifest);

    int n = write(newsockfd, man, strlen(man));
    if (n < 0) {

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
    }

    char* comF = readSock(newsockfd);

    if (strstr(comF, "Error") != NULL) {

        fprintf(stderr, "%s\n", comF);
        //////close(sockfd);
        free(comF);
        close(newsockfd);
        free(man);
        return -1; //exit(-1);
    }

    char com[strlen("repository/") + (strlen(proj)) + strlen(".Commit/") + 1];      //done
    com[strlen("repository/") + (strlen(proj)) + strlen(".Commit/")] = '\0';        //done
    strcpy(com, "repository/");
    strcat(com, proj);
    strcat(com, "/");
  //  strcat(com, proj);
    strcat(com, ".Commit");

    int cfd = open(com, O_RDWR | O_TRUNC | O_CREAT, 00600);
    if (cfd < 0) {
        int wr = write(newsockfd, "Error: unable to open server commit to complete successful commit", strlen("Error: unable to open server commit to complete successful commit"));
        if (wr < 0) {    

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
    }

    int writeIn = 0;
    int status = 0;
    do { 
        status = write(cfd, comF + writeIn, strlen(comF) - writeIn);
        if (status < 0 && writeIn == 0) {
            int wr = write(newsockfd, "Error: unable to open server commit to complete successful commit", strlen("Error: unable to open server commit to complete successful commit"));
            if (wr < 0) {    

                fprintf(stderr, "Fatal Error: unable to write to socket\n");
            }
        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < strlen(comF));
    close(cfd);


    int wr = write(newsockfd, "Successfully committed", strlen("Successfully committed"));
    if (wr < 0) {    

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
    }


    free(man);
    free(comF);
    return 1;
}


int push(int sockfd, int newsockfd, char* proj) {

    // initial response

    char projPath[strlen("repository/") + strlen(proj) + 1];
    projPath[strlen("repository/") + strlen(proj)] = '\0';
    strcpy(projPath, "repository/");
    strcat(projPath, proj);

    DIR* pd = opendir(projPath);
    if (pd == NULL) {

        int n = write(newsockfd, "Error: project does not exist in server repository\n", strlen("Error: project does not exist in server repository\n"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    closedir(pd);

    char comm[strlen(projPath) + strlen("/")  + strlen(".Commit") + 1]; //done
    comm[strlen(projPath) + strlen("/") + strlen(".Commit")] = '\0';    //done
    strcpy(comm, projPath);
    strcat(comm, "/");
  //  strcat(comm, proj);
    strcat(comm, ".Commit");

    int cfd = open(comm, O_RDONLY);
    if (cfd < 0) {

        int n = write(newsockfd, "Error: server .Commit file unable to be opened or does not exist\n", strlen("Error: server .Commit file unable to be opened or does not exist\n"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    close(cfd);

    char* sCom = fillBuff(comm);


    if (write(newsockfd, sCom, strlen(sCom)) < 0) {

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
        ////close(sockfd);
        close(newsockfd);
        free(sCom);
        return -1; //exit(-1);
    }
    // initial response


    int length = findBytes(newsockfd);
    char* tarfile = readSockwithLen(newsockfd, length);

   
    //if (strstr(tarfile, "Error") != NULL) {

        //fprintf(stderr, "%s", tarfile);
    //}

    int tarfd = open("temp.tar.gz", O_RDWR | O_TRUNC | O_CREAT, 00600);

    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(tarfd, tarfile + writeIn, length - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(tarfd);
            ////close(sockfd);
            close(newsockfd);
            return -1; //exit(-1);
        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < length);

    close(tarfd);
    
   

    // untar and send to backup

    char manPath[strlen(projPath) + strlen("/")  + strlen(".Manifest") + 1]; //done
    manPath[strlen(projPath) + strlen("/") + strlen(".Manifest")] = '\0'; //done
    strcpy(manPath, projPath);
    strcat(manPath, "/");
  //  strcat(manPath, proj);
    strcat(manPath, ".Manifest");


    char* tempMan = fillBuff(manPath);
    char* vNum = strtok(tempMan, "\n");

    char backTarPath[strlen("repository/backups/") + strlen(proj) + strlen("/") + strlen(proj) + strlen(vNum) + strlen(".tar.gz") + 1]; 
    backTarPath[strlen("repository/backups/") + strlen(proj) + strlen("/") + strlen(proj) + strlen(vNum) + strlen(".tar.gz")] = '\0';
    strcpy(backTarPath, "repository/backups/");
    strcat(backTarPath, proj);
    strcat(backTarPath, "/");
    strcat(backTarPath, proj);
    strcat(backTarPath, vNum);
    strcat(backTarPath, ".tar.gz"); // fix this


    char compressCom[strlen("tar czf ") + strlen(backTarPath) + strlen(" -C ./repository ./") + strlen(proj) + 1];  // tar czf repository/backups/testProj/testProj.tar.gz -C ./repository ./testProj
    compressCom[strlen("tar czf ") + strlen(backTarPath) + strlen(" -C ./repository ./") + strlen(proj)] = '\0';
    strcpy(compressCom, "tar czf ");
    strcat(compressCom, backTarPath);
    strcat(compressCom, " -C ./repository ./");
    strcat(compressCom, proj);

    // tar czf repository/backups/proj/proj0.tar.gz -C ./repository/proj

    //make entry into history before tar with server commit file

    char entry[strlen(sCom) + 1];
    bzero(entry, strlen(sCom) + 1);
    entry[strlen(sCom)] = '\0';

    char* curr = strtok(sCom, "\n");

    strcpy(entry, curr);

    curr = strtok(NULL, "\n");

    while(curr != NULL) {

        char* flag = strstr(curr, "\t");

        if (flag[1] != 'I' && flag[1] != 'R' && flag[1] != 'S') {

            strcat(entry, "\n");
            strcat(entry, curr);

        }
    
        curr = strtok(NULL, "\n");
        
    } 
    

    char hist[strlen("repository/backups/") + ( strlen(proj)) + strlen(".History/") + 1]; //done
    hist[strlen("repository/backups/") + ( strlen(proj)) + strlen(".History/")] = '\0'; //done
    strcpy(hist, "repository/backups/");
    strcat(hist, proj);
    strcat(hist, "/");
  //  strcat(hist, proj);
    strcat(hist, ".History");

     

    int hfd = open(hist, O_RDONLY);
    if (hfd > 0) {
        
        close(hfd);

        char* oldhis = fillBuff(hist);
        char newHis[strlen(oldhis) + strlen("\n\n") + strlen(entry) + 1];
        newHis[strlen(oldhis) + strlen("\n\n") + strlen(entry)] = '\0';
        strcpy(newHis, oldhis);
        strcat(newHis, "\n\n");
        strcat(newHis, entry);

        hfd = open(hist, O_RDWR | O_TRUNC | O_CREAT, 00600);

        writeIn = 0;
        status = 0;
        do{ 
            status = write(hfd, newHis + writeIn, strlen(newHis) - writeIn);
            if (status < 0 && writeIn == 0) {
                fprintf(stderr, "Fatal Error: could not write to file\n");
                close(hfd);
                ////close(sockfd);
                close(newsockfd);
                return -1; //exit(-1);
            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(newHis));

        close(hfd);
        free(oldhis);

    }
    else {

        close(hfd);

        hfd = open(hist, O_RDWR | O_TRUNC | O_CREAT, 00600);

        writeIn = 0;
        status = 0;
        do{ 
            status = write(hfd, entry + writeIn, strlen(entry) - writeIn);
            if (status < 0 && writeIn == 0) {
                fprintf(stderr, "Fatal Error: could not write to file\n");
                close(hfd);
                ////close(sockfd);
                close(newsockfd);
                return -1; //exit(-1);
            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(entry));

        close(hfd);
    }
    

    free(tempMan);
    free(sCom);
    // entry

    remove(comm); //first remove the commit file before going into storage


    //printf("%s\n", compressCom);
    if (system(compressCom) < 0) {  //this line now stores the project

        fprintf(stderr, "Fatal Error: unable to store current project verison in backups\n");
    }

    recPrac(projPath, remove);   //remove server side project before untarring


    //untar sent project which is now in temp.tar.gz

    if (system("tar xzf temp.tar.gz -C repository") < 0) {

        fprintf(stderr, "Fatal Error: unable to untar pushed project\n");
    }

    remove("temp.tar.gz");
    //delete old version and temp.tar.gz



    if (write(newsockfd, "Successful push", strlen("Successful push")) < 0) {

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
        free(sCom);
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    //*/

    return 1;
}


int create(int newsockfd, char* proj) {

    char fullP[14 + strlen(proj)];
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP);

    if (in != NULL) {

        closedir(in); 
        return -1;
    }


    mkdir(fullP, 00700);
    char manifest[strlen(fullP)  + strlen(".Manifest") + 1]; //done
    strcpy(manifest, fullP);
    strcat(manifest, "/");
 //   strcat(manifest, proj);
    strcat(manifest, ".Manifest");

    printf("manifest = \"%s\"\n", manifest);
    int fd = open(manifest, O_RDWR | O_TRUNC | O_CREAT, 00600); //  provide error case for bad read


    char back[strlen("repository/backups/") + strlen(proj) + 1];
    back[strlen("repository/backups/") + strlen(proj)] = '\0';
    strcpy(back, "repository/backups/");
    strcat(back, proj);

    mkdir(back, 00700);


    write(fd, "0", 1);                 //  provide error case for bad write
    write(newsockfd, "0", 1);

    close(fd);
    return 1;
}


int destroy(char* proj) {

    char fullP[14 + strlen(proj)];
    strcpy(fullP, "./repository/");
    strcat(fullP, proj);

    DIR* in = opendir(fullP); 

    if (in == NULL)
        return -1;

    closedir(in);
    recPrac(fullP, remove);

    char back[strlen("repository/backups/") + strlen(proj) + 1];
    back[strlen("repository/backups/") + strlen(proj)] = '\0';
    strcpy(back, "repository/backups/");
    strcat(back, proj);

    recPrac(back, remove);

    return 1;
}



int currentversion(int sockfd, int newsockfd, char* proj) {

    char projPath[ (strlen(proj)) + strlen("./repository/") + strlen(".Manifest") + 2]; //done wierd formatting
    projPath[strlen(proj) + strlen("./repository/") + 1] = '\0';
    strcpy(projPath, "./repository/");
    strcat(projPath, proj);
    strcat(projPath, "/");
  //  strcat(projPath, proj);
    strcat(projPath, ".Manifest");

    int fd = open(projPath, O_RDWR);
    if (fd < 0) {

        int n = write(newsockfd, "Error: cannot open project manifest\n", strlen("Error: cannot open project manifest\n"));

        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            close(newsockfd);
            ////close(sockfd);
            return -1; //exit(-1);;
        } 
    }
    close(fd);

    char* manifest = fillBuff(projPath); //printf("Manifest:\n%s\n\n\n", manifest);
    char retMes[strlen(manifest) + 1];
    bzero(retMes, strlen(manifest) + 1);
    retMes[strlen(manifest)] = '\0';
    strcpy(retMes, proj);
    strcat(retMes, "\t");

    int c = 1;
    char* curr = strtok(manifest, "\n");
    strcat(retMes, curr);
    strcat(retMes, "\n");
    int num = 0;

    curr = strtok(NULL, "\t"); 
    while (curr != NULL) {
        
        //printf("curr is '%s'\n", curr);

        if (strstr(curr, proj) != NULL) {

            strcat(retMes, curr);
            strcat(retMes, "\t");
            curr = strtok(NULL, "\t");
        }
        else if (isNumber(curr) == 1 && strcmp(curr, "S") != 0) {
            
            num = 1;
            strcat(retMes, curr);
            strcat(retMes, "\n");
            curr = strtok(NULL, "\n");
        }
        else {
            
            curr = strtok(NULL, "\t");

        }
            
    }

    strcat(retMes, "\0");
    //printf("\n\nretMes:\n%s", retMes);

    int n = write(newsockfd, retMes, strlen(retMes));

    if (n < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(newsockfd);
        ////close(sockfd);
        return -1; //exit(-1);;
    }

    free(manifest);
    return 1;
}


int history(int sockfd, int newsockfd, char* proj) {

    char projPath[ strlen(proj)+ strlen("./repository/") + 1];
    projPath[strlen(proj)+ strlen("./repository/")] = '\0';
    strcpy(projPath, "./repository/");
    strcat(projPath, proj);

    DIR* pd = opendir(projPath);

    if (pd == NULL) {

        int n = write(newsockfd, "Error: project does not exist in server repository", strlen("Error: project does not exist in server repository"));
        if (n < 0) {

            fprintf(stderr, "Error: unable to write to socket\n");
        }
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }


    char hist[strlen("repository/backups/") + (strlen(proj)) + strlen(".History/") + 1]; //done
    hist[strlen("repository/backups/") + (strlen(proj)) + strlen(".History/")] = '\0'; //done
    strcpy(hist, "repository/backups/");
    strcat(hist, proj);
    strcat(hist, "/");
  //  strcat(hist, proj);
    strcat(hist, ".History");

    char* hi = fillBuff(hist);

    int n = write(newsockfd, hi, strlen(hi));
    if (n < 0) {

        fprintf(stderr, "Error: unable to write to socket\n");
        ////close(sockfd);
        close(newsockfd);
        free(hi);
        return -1; //exit(-1);
    }


    free(hi);
    return 1;
}


int rollback(int sockfd, int newsockfd, char* proj, char* vers) {

    int v1 = atoi(vers);

    char projPath[strlen("repository/") + strlen(proj) + 1];
    projPath[strlen("repository/") + strlen(proj)] = '\0';
    strcpy(projPath, "repository/");
    strcat(projPath, proj);

    int checkProj = open(projPath, O_RDONLY);
    if (checkProj < 0) {

        int n = write(newsockfd, "Error: project does not exist on server repository", strlen("Error: project does not exist on server repository"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    close(checkProj);

    char src[strlen("repository/backups/") + (2 * strlen(proj)) + strlen(vers) + strlen(".tar.gz/") + 1];
    src[strlen("repository/backups/") + (2 * strlen(proj)) + strlen(vers) + strlen(".tar.gz/")] = '\0';
    strcpy(src, "repository/backups/");
    strcat(src, proj);
    strcat(src, "/");
    strcat(src, proj);
    strcat(src, vers);
    strcat(src, ".tar.gz");


    char tarcommand[strlen("tar xzf ") + strlen(src) + strlen(" -C repository") + 1];
    tarcommand[strlen("tar xzf ") + strlen(src) + strlen(" -C repository")] = '\0';
    strcpy(tarcommand, "tar xzf ");
    strcat(tarcommand, src);
    strcat(tarcommand, " -C repository");

    char remProj[strlen("rm -R repository/") + strlen(proj) + 1];
    remProj[strlen("rm -R repository/") + strlen(proj)] = '\0';
    strcpy(remProj, "rm -R repository/");
    strcat(remProj, proj);


    int checkSrc = open(src, O_RDONLY);
    if (checkSrc < 0) {

        int n = write(newsockfd, "Error: project version is invalid", strlen("Error: project version is invalid"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }
    //nfs edit
   // close(checkSrc);
    close(checkProj);


    system(remProj); // remove project currently in repo
    system(tarcommand); // untar project backup to repo
    int suc = remove(src);

    // delete all versions >= version number and stop when remove fails

    // search 
    //char* newVers[strlen(vers) + 1];
    //newVers[strlen(vers)] = '\0';


    while (suc != -1) {

        v1++;

        char nVers[6];
        bzero(nVers, 6);
        nVers[5] = '\0';
        sprintf(nVers, "%d", v1);

        char currT[strlen("repository/backups/") + (2 * strlen(proj)) + strlen(nVers) + strlen(".tar.gz/") + 1];
        currT[strlen("repository/backups/") + (2 * strlen(proj)) + strlen(nVers) + strlen(".tar.gz/")] = '\0';
        strcpy(currT, "repository/backups/");
        strcat(currT, proj);
        strcat(currT, "/");
        strcat(currT, proj);
        strcat(currT, nVers);
        strcat(currT, ".tar.gz");

        suc = remove(currT);
    }

    //update history file to not include anything above version number

    char hist[strlen("repository/backups/") + (strlen(proj)) + strlen(".History/") + 1]; //done
    hist[strlen("repository/backups/") + (strlen(proj)) + strlen(".History/")] = '\0'; //done
    strcpy(hist, "repository/backups/");
    strcat(hist, proj);
    strcat(hist, "/");
  //  strcat(hist, proj);
    strcat(hist, ".History");

    char* his = fillBuff(hist);

    char newHis[strlen(his) + strlen("\n\nrolled back to ") + strlen(vers) + 1];
    newHis[strlen(his) + strlen("\n\nrolled back to ") + strlen(vers)] = '\0';
    strcpy(newHis, his);
    strcat(newHis, "\n\nrolled back to ");
    strcat(newHis, vers);

    int hfd = open(hist, O_RDWR | O_TRUNC);
    if (hfd < 0) {

        int n = write(newsockfd, "Error: unable to open project history file", strlen("Error: unable to open project history file"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
            free(his);
            ////close(sockfd);
            close(newsockfd);
            return -1; //exit(-1);
        }
    }


    int writeIn = 0;
    int status = 0;
    do { 
        status = write(hfd, newHis + writeIn, strlen(newHis) - writeIn);
        if (status < 0 && writeIn == 0) {
            int wr = write(newsockfd, "Error: unable to open server commit to complete successful commit", strlen("Error: unable to open server commit to complete successful commit"));
            if (wr < 0) {    

                fprintf(stderr, "Fatal Error: unable to write to socket\n");
            }
        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < strlen(newHis));

    close(hfd);

    int n = write(newsockfd, "Successfully rolledback to desired version number", strlen("Successfully rolledback to desired version number"));
    if (n < 0) {

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
        free(his);
        ////close(sockfd);
        close(newsockfd);
        return -1; //exit(-1);
    }

    free(his);
    return 1;
}







#endif