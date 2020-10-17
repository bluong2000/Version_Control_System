#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h> 
#include <libgen.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h> 
//#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/evp.h>
#include <openssl/opensslconf.h>
#include <openssl/crypto.h>
#include <openssl/sha.h>
#include <openssl/opensslv.h>
#include <time.h>

void convert(unsigned char _hash[],unsigned char __hash[]){
    int i =0;
    for(i; i<20; i++)
    {
     //   printf("%02x", __hash[i]);
        sprintf(&_hash[i], "%02x", __hash[i] );
    }
    printf("\n");
  //  printf("hashx is:%s\n",_hash );
   // printf("hashraw is:%s\n",__hash );
}
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
    //printf("\nDir: %s\n", f);
    int x = fun(f);
        if (x != 0)
            printf("Dir not removed\n");

    return;
}




int getMessage(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 400)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 400 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(sockfd);
        return -1; //exit(-1);

return -1;

    }

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                close(sockfd);
                return -1; //exit(-1);

return -1;
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);


    printf("%s\n", buff);

    return 1;
}





char* readSock(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 400)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 40 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(sockfd);
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
                close(sockfd);
                return NULL; //exit(-1);

            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

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
                close(sockfd);
                return NULL; //exit(-1);

            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

    return buff;
}



int findBytes(int sockfd) {

    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 40)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
    //   printf("%d\t", c);
       delay(1);
       c++;
    }

    if (c == 40 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        
        close(sockfd);
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



char* fillBuff (char* file) {

    int mc = 0;

    int fTemp = open(file, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);

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


int conn() {

    int fd = open(".configure", O_RDONLY);
    if (fd < 0) {

        fprintf(stderr, "Fatal Error: could not open .config file and cannot establish a connection to the server, the file may not exist\n");
        return -1; //exit(-1);

    }
    close(fd);

    char* buff = fillBuff(".configure");
    char* IP = strtok(buff, "\t");
    char* portno = strtok(NULL, "\t");

    int sockfd;
    struct sockaddr_in serverAddressInfo;
    struct hostent *serverIPAddress;


    serverIPAddress = gethostbyname(IP); //maybe need to free this hostent, unsure if it is malloced with gethostbyname()
    if (serverIPAddress == NULL) {

        fprintf(stderr, "Fatal Error: no such host: \"%s\"\n", IP);
        free(buff);
        return -1; //exit(-1);

    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {

        fprintf(stderr, "Fatal Error: could not construct a socket to establish connection\n");
        free(buff);
        return -1; //exit(-1);

    }

    bzero( (char*) &serverAddressInfo, sizeof(serverAddressInfo));
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_port = htons(atoi(portno));
    bcopy( (char*)serverIPAddress->h_addr, (char*)&serverAddressInfo.sin_addr.s_addr, serverIPAddress->h_length);

    struct sockaddr *sai = (struct sockaddr *) &serverAddressInfo;
    if ( connect(sockfd, sai, sizeof(serverAddressInfo)) < 0 ) {

        fprintf(stderr, "Fatal Error: could not establish connection possibly incorrect port or host\n");
        free(buff);
        close(sockfd);
        return -1; //exit(-1);

    }


   /* 

    if (write(sockfd, "baeyroi\n", 8) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    */
    printf("connection established\n");
    free(buff);
    return sockfd;
}


int configure(char* ip, char* port) {

    int fd = open(".configure" , O_RDWR | O_TRUNC | O_CREAT, 00600);

    if (fd < 0) {

        fprintf(stderr, "Fatal Error: could not open or create file\n");
        return -1; //exit(-1);

    }

    int ipSize = strlen(ip);
    int portSize = strlen(port);


    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(fd, ip + writeIn, ipSize - writeIn);

        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < ipSize);

    
    status = write(fd, "\t", 1);
    if (status < 0) {
        fprintf(stderr, "Fatal Error: could not write to file\n");
        close(fd);
        return -1; //exit(-1);

    }

    writeIn = 0;
    status = 0;
    do{ 
        status = write(fd, port + writeIn, portSize - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < portSize);

    close(fd);
    printf("succesfully created configure file\n");
    //conn(); // keep for testing conn but remove later
    //system("mv testPath/proj1/ testPath/Backups/");

    return 1;
}


int checkout(char* proj) {

    int sockfd = conn();

    char message[9 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "checkout\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }


    int len = findBytes(sockfd);
    char* bin = readSockwithLen(sockfd, len);
    if (strstr(bin, "Error") != NULL) {
        fprintf(stderr, "%s\n", bin);
        close(sockfd);
        return -1; //exit(-1);

    }

    int fd = open("temp.tar.gz", O_RDWR | O_TRUNC | O_CREAT, 00600);

    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(fd, bin + writeIn, len - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            close(sockfd);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < len);

    close(fd);


    if (system("tar xzf temp.tar.gz") == -1) {

        fprintf(stderr, "Fatal Error: could not extract tarred file\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    remove("temp.tar.gz");

    char com[(strlen(proj)) + strlen(".Commit/") + 1]; //done
    com[(strlen(proj)) + strlen(".Commit/")] = '\0';//done
    strcpy(com, proj);
    strcat(com, "/");
  //  strcat(com, proj);
    strcat(com, ".Commit");
    remove(com);

    printf("Successfully downloaded project file\n");

    free(bin);
    close(sockfd);

    return 1;
}


int update(char* proj) {
  
    // checks if project exists in server directory
    int sockfd = conn();

    char message[9 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "update\t");
    strcat(message, proj);
    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }


    //prepares socket to recieve message loads manifest into bin array
    int len = findBytes(sockfd);

    char* sbuff = readSockwithLen(sockfd, len);
    if (strstr(sbuff, "Error") != NULL) {
        fprintf(stderr, "%s\n", sbuff);
        close(sockfd);
        return -1; //exit(-1);

    }


    //copies over server manifest bc manifest/project  was found
    char sman[strlen("server") + strlen(proj) + strlen(".//.Manifest") + 1];  //done  //  length of "./" + ".Manifest + length of proj" //turned into .serverManifest
    sman[strlen("server") +strlen(proj) + strlen(".//.Manifest")] = '\0';//done

    strcpy(sman, "./");
    strcat(sman, proj);
    strcat(sman, "/");
 //   strcat(sman, "server");
    strcat(sman, ".serverManifest");

    int fd = open(sman, O_RDWR | O_TRUNC | O_CREAT, 00600);
    
    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(fd, sbuff + writeIn, strlen(sbuff) - writeIn);

        if (status < 0 && writeIn == 0) { 
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            close(sockfd);
            return -1; //exit(-1);

        }
        writeIn += status;
    }while (status > 0 && writeIn < strlen(sbuff));

    close(fd);
  // printf("sbuff:%s\n", sbuff);
    //creates buff of client's manifest to compare with the server's manifest  
    char man[strlen(proj)  + strlen(".//.Manifest") + 1]; //done
    man[strlen(proj)  + strlen(".//.Manifest")] = '\0'; //done
    strcpy(man, "./");
    strcat(man, proj);
    strcat(man, "/");
 //   strcat(man, proj);
    strcat(man, ".Manifest");
    char * cbuff = fillBuff(man);
  //  printf("%s\n", man);
  // printf("cbuff:%s\n", cbuff);
 //   printf("sbuff:%s\n", sbuff);

    char conf[strlen(proj)  + strlen(".//.Conflict") + 1]; //done
    man[strlen(proj)  + strlen(".//.Conflict")] = '\0'; //done  //changed proj to server
    strcpy(conf, "./");
    strcat(conf, proj);
    strcat(conf, "/");
   // strcat(conf, proj);
    strcat(conf, ".Conflict");
    int rem = remove(conf);
    //creates empty update file
    char update[strlen(proj) +  strlen(".//.Update") + 1];//done
    man[strlen(proj) + strlen(".//.Update")] = '\0';//done
    strcpy(update, "./");
    strcat(update, proj);
    strcat(update, "/");
//    strcat(update, proj);
    strcat(update, ".Update");
 //   printf("%s\n" , update);
    //removes any preexisting update files
    remove(update);

    fd = open(update, O_RDWR | O_TRUNC | O_CREAT, 00600);
    close(fd);
    int cflag = 0;
   // printf("cbuff:%s\n", cbuff);
 //   printf("sbuff:%s\n", sbuff);
    if (strcmp(sbuff, cbuff) == 0 ){
        //manifests are the same(Full sucess)
        printf("Up To Date\n");

        char remov[strlen(proj)  + strlen(".//.Conflict") + 1]; //done
        man[strlen(proj) + strlen(".//.Conflict")] = '\0'; //done
        strcpy(remov, "./");
        strcat(remov, proj);
        strcat(remov, "/");
    //    strcat(remov, proj);
        strcat(remov, ".Conflict");
     //   printf("%s\n" , remov);
        int rem = remove(remov);


    } else{ 
        //manifests  are different
     //   printf("manifests are different\n");
        //loop through server's manifest and get file paths buffs: sbuff, cbuff

        //breaks it into every newline
      //  printf("%s\n", sbuff);
       int ssize = strlen(sbuff);
      


        //creates update buffer and conflict buffer
        int buffsize = ssize+strlen(cbuff)+1;
        char ubuff[ssize+strlen(cbuff)+1];
        ubuff[ssize+strlen(cbuff)] = '\0';
        bzero(ubuff, strlen(ubuff));

        char conbuff[strlen(cbuff)+1];
        conbuff[strlen(cbuff)]= '\0';
        bzero(conbuff, strlen(conbuff));
        char * line = strtok(sbuff, "\n");
        strcat(ubuff, line);
        strcat(ubuff,"\n");

    //    printf("first line is:%s\n", line);
        if(line == NULL){
            return;
        }
        line = strtok(NULL, "\n");  //skips over project version  
        int nullflag = 0;
        if(line == NULL){
            printf("here \n");

           // return;
       

        } 

  //      char entryl[strlen(line)+1];
   //     char path[strlen(line)-23];
    //    path[strlen(line)-22] = '\0'; 

        char hash[21];
        hash[20] = '\0';
      //  if(line == NULL){
       //     printf("null is here");
       // }
       if(line != NULL){
          char entryl[strlen(line)+1];
         char path[strlen(line)-23];
          path[strlen(line)-22] = '\0'; 
        while (line != NULL){ //loops through every filepath in the server manifest
            //    printf("server line is: %s\n", line);
     
            if(line != NULL){
        //        printf("2calc line is: %s\n", line);
                //gets path of file
                int i = 0;
                char str[2] = "\0";
                str[0] = line[i];
                while ( strcmp( str, "\t")!= 0){
                    i++;
                    strcat(path, str);
                    str[0] = line[i];
                }
                path[i] = '\0';
             
     //           printf("path is %s\n", path);
                
                char * p1 = strstr(cbuff, path);
  

                if(p1){ //check if file exists in client manifest
             //       printf("%s exists in  both manifests\n", path);

                    char* h = strrchr(line, '\t');
                    strcpy(hash, h+1);
                    hash[20] = '\0';

        //            printf("hash is:%s\n", hash);

                    //check if hashes are the same
                    //copy manifest
                    
                    char man2[strlen(proj)  + strlen(".//.Manifest") + 1];//done
                    man2[strlen(proj)  + strlen(".//.Manifest")] = '\0';//done
                    strcpy(man2, "./");
                    strcat(man2, proj);
                    strcat(man2, "/");
            //        strcat(man2, proj);
                    strcat(man2, ".Manifest");
           //         printf("file:%s\n", man2);
                    char* tempman= fillBuff(man2);
                   int tempsize = strlen(tempman);
                    tempman[buffsize-1] = '\0';
                    char * p10 =  strstr(tempman, path);
                    char* templine = strsep(&p10, "\n");
            //        printf("manifest line:%s\n", templine);


                    char * p11 = strstr(templine, hash);


                    if(p11){
                        //hashes are the same file was not modified
                        //file is the same add it unchanged to the update buffer
                 //       printf("line is: %s\n", line);
                    //    printf("templine is\n", templine);
             //           strcat(ubuff,line);
              //          strcat(ubuff, "hi");
                        
               //         strcat(ubuff,"\n");
                 //       printf("hashes are the same\n");
               //        printf("ubuff is:%s\n", ubuff);

                    }else{
                        //manifests are diff: find live hash of file

              //          printf("server and client hashes are different\n");
                            //creates a hash for the file

                        char* file = fillBuff(path);
                 //       printf("here");
                        size_t length = strlen(file);
                        unsigned char livehash[20];
                        bzero(livehash, 20);
                        unsigned char livehashbin[20];
                        bzero(livehashbin, 20);

                        bzero(livehashbin,strlen(livehashbin));
                        bzero(livehash,strlen(livehash));

                        SHA1(file, length, livehashbin);
                        convert(livehash,livehashbin);
                        livehash[20] = '\0';
              //          printf("live hash is:%s\n", livehash);


                        //checks if live hash is the same as client's hash
                        char *p3 = strstr(templine, livehash);
                   
                        if(p3){
                            //hashes are the same: modified file
                     //       printf("live and client has are same\n");

                            char * flag = strstr(line, "\t");
                            flag[1] = 'M';
                            printf("M %s\n", path);
                            //after this path gets reky for some reason
                            strcat(ubuff,line);
                            strcat(ubuff, "\n");

                            bzero(line,strlen(line));

           //                 printf("ubuff:%s\n", ubuff);



                        }else{

                            //hashes are different: conflict file sets conflict flag
                //           printf("live and client are different");
                            cflag = 1;
                            printf("C %s\n", path);
                            //char path2[strlen(path)+1];
                            //path2[strlen(path)] = '\0';
                           // strcpy(path2,path);

                            //create line with file path, client version, live hash
            //                printf("line size is %d:\n", strlen(line));
            
                            char conline[strlen(line)+ 10]; //why is there not enought room?
 //                           conline[strlen(line)+10] = '\0';
                            bzero(conline, strlen(line)+ 10);
                            strcat(conline, path);
                            int consize = strlen(conline);
           //                 printf("conline size is %d:\n", consize);
                            strcat(conline, "\t");
                          strcat(conline, "C");
                            strcat(conline, "\t");
                            char * ptr4 = strstr(cbuff, path);
                            int i = 3;
          //                  printf("this is conline:%s\n", conline);
                            char str[2];
                            str[1] = '\0' ;
                     //       str[0] = line[i];
                 //           printf("this is conline:%s\n", conline);

                            while( ptr4[strlen(path)+i] != '\t'){
                      //      while (strcmp(ptr4[i], "\t")!= 0){
                   //             printf("this is char: %c :\n", ptr4[strlen(path)+i]);

                                str[0] = ptr4[strlen(path) + i];
                                strcat(conline,str);
            //                    printf("this is conline:%s\n", conline);

                                i++;
                            }
                        //    conline[strlen(path) +i] = '\0';
                            strcat(conline,"\t");
                            strcat(conline,livehash);
                            int arsize;
                            arsize = strlen(path)+i;
                            int hashsize = strlen(livehash);
                  //          int consize = sizeof(conline)/sizeof(conline[0]);
                            consize = strlen(conline);
                //            printf("conline size is %d:\n", consize);

                  //          printf("arsize size is %d:\n", arsize);
                    //        printf("livehash size is %d:\n", hashsize);


                           // printf("this is char: %c :\n", ptr4[strlen(path)+5]);
                 //          printf("this is conline:%s\n", conline);
               
                           strcat(conbuff, conline);
                          strcat(conbuff, "\n");
                            bzero(line,strlen(line));


   



                        }



                            


                    }




                }else{ //file does not exist in client manifests so it was added!

                    char * flag = strstr(line, "\t");
                    flag[1] = 'A';
                    printf("A %s\n", path);
                    //after this path gets reky for some reason
                //    strcat(ubuff, "\n");
                    strcat(ubuff,line);
         //           printf("new entry:%s\n", line);

                    bzero(line,strlen(line));

         //           printf("ubuff:%s\n", ubuff);




                }


                bzero(path, strlen(path)-1);
 
            }
            line = strtok(NULL, "\n");  //skips over project version



         }
        }
         //end of while loop
        //done going through server manifest: now going through client manifest
  //      printf("cbuff: %s\n", cbuff);
        char * line2 = strtok(cbuff, "\n");
        char * s2buff = fillBuff(sman);
     
        line2 = strtok(NULL, "\n");  //skips over project version
    //    char entryl[strlen(line)+1];
        if(line2 != NULL) {
        char path2[strlen(line2)-23]; 
     

        while (line2 != NULL){ //loops through every filepath
            if(line2 != NULL){
        //        printf("line2 is: %s\n", line2);
                //gets path of file
                int i = 0;
                char str[2] = "\0";
                str[0] = line2[i];
                while ( strcmp( str, "\t")!= 0){
                    i++;
                    strcat(path2, str);
                    str[0] = line2[i];
                }
                path2[i] = '\0';
             
     //           printf("path2 is %s\n", path2);
               
                char * p5 = strstr(s2buff, path2);
         //       printf("s2buff is %s\n", s2buff);
                

                if(!p5){
       //             printf("path2 is not found\n");
                     //it does not exist in server so it was removed
                    char * flag2 = strstr(line2, "\t");
                    flag2[1] = 'R';
                    printf("D %s\n", path2);
                    //after this path gets reky for some reason
                    strcat(ubuff, "\n");
                    strcat(ubuff,line2);
       //             printf("new entry:%s\n", line2);

                    bzero(line2,strlen(line2));

        //            printf("ubuff:%s\n", ubuff);            
             
                }
            

              //  if(p5){ //check if file exists in server manifest


                //}else{
                    //it does not exist in server so it was removed

            bzero(path2, strlen(path2)-1);

            }

            line2 = strtok(NULL, "\n");  //skips over project version




        }
        }
        //end of second loop

        fd = open(update, O_RDWR | O_TRUNC, 00600);
   //     printf("strlen ubuff:%d\n", strlen(ubuff));
        int writeIn = 0;
        int status = 0;
        do{ 
            status = write(fd, ubuff + writeIn, strlen(ubuff) - writeIn);

            if (status < 0 && writeIn == 0) { 
                fprintf(stderr, "Fatal Error: could not write to file\n");
                close(fd);
                return -1; //exit(-1);

            }
            writeIn += status;
        }while (status > 0 && writeIn < strlen(ubuff));
        
        close(fd);
        if (cflag == 1){  // a conflict occured
            remove(update);
            //make conflict file
            char conflict[strlen(proj)  + strlen(".//.Conflict") + 1]; //done
            man[strlen(proj) + strlen(".//.Conflict")] = '\0'; //done
            strcpy(conflict, "./");
            strcat(conflict, proj);
            strcat(conflict, "/");
       //     strcat(conflict, proj);
            strcat(conflict, ".Conflict");
         //   printf("%s\n" , conflict);
                
            fd = open(conflict, O_RDWR | O_TRUNC | O_CREAT, 00600);
    
            int writeIn = 0;
            int status = 0;
            do{ 
                status = write(fd, conbuff + writeIn, strlen(conbuff) - writeIn);

                if (status < 0 && writeIn == 0) { 
                    fprintf(stderr, "Fatal Error: could not write to file\n");
                    close(fd);
                    return -1; //exit(-1);

                }
                writeIn += status;
            }while (status > 0 && writeIn < strlen(conbuff));
            
            close(fd);
            //delete update file
            printf("Conflicts found: Must resolve conflicts before project can be updated\n");



        }

    //    printf("final project update buffer:%s\n", ubuff);
   //     printf("final project conflict buffer:%s\n" , conbuff);


 

    }
 //  printf("%s\n", sman);
    int remman = remove(sman);


    close(fd);
    if(cflag == 0){
        printf("Update completed\n");
    }


    return 1;
}


int upgrade(char* proj) {

    char upFile[strlen(proj) + strlen("/")  + strlen(".Update") + 1]; //done
    upFile[strlen(proj) + strlen("/") + strlen(".Update")] = '\0'; //done
    strcpy(upFile, proj);
    strcat(upFile, "/");
 //   strcat(upFile, proj);
    strcat(upFile, ".Update");

    char conFile[strlen(proj) + strlen("/") +  strlen(".Conflict") + 1]; //done
    conFile[strlen(proj) + strlen("/") + strlen(".Conflict")] = '\0'; //done
    strcpy(conFile, proj);
    strcat(conFile, "/");
  //  strcat(conFile, proj);
    strcat(conFile, ".Conflict");

//    printf("upFile = %s\n", upFile);
    int _up = open(upFile, O_RDONLY);
    if (_up < 0) {

        fprintf(stderr, "Error: no update file, do an update before upgrading\n");
        return -1; //exit(-1);

    }
    close(_up);

    char* upBuff = fillBuff(upFile);

    if (strlen(upBuff) == 0) {

        fprintf(stderr, "Project is up to date\n");
        remove(upFile);
        //free(upBuff);
        return -1; //exit(-1);

    }


    int _con = open(conFile, O_RDONLY);
    if (_con > 0) {

        close(_con);
        fprintf(stderr, "Error: must resolve conflicts and then update in order to upgrade\n");
        return -1; //exit(-1);

    }



    int sockfd = conn();

    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "upgrade\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }



    char* sMan = readSock(sockfd);

    if (strcmp(sMan, "Error: project does not exist in server repository\n") == 0) {

        fprintf(stderr, sMan, strlen(sMan));
        close(sockfd);
        free(upBuff);
        return -1; //exit(-1);

    }

    char mani[strlen(proj) + strlen("/")  + strlen(".Manifest") + 1]; //done
    mani[strlen(proj) + strlen("/")  + strlen(".Manifest")] = '\0'; //done
    strcpy(mani, proj);
    strcat(mani, "/");
  //  strcat(mani, proj);
    strcat(mani, ".Manifest");

    char* cMan = fillBuff(mani);



    char newMan[strlen(cMan) + strlen(sMan) + 1];
    bzero(newMan, strlen(cMan) + strlen(sMan) + 1);
    newMan[strlen(cMan) + strlen(sMan)] = '\0';
    strcpy(newMan, sMan);


    char* curr = strtok(cMan, "\n");
    curr = strtok(NULL, "\n");


    while(curr != NULL) {

        char* p = strsep(&curr, "\t");
        char* upEntry = strstr(upBuff, p);
        char* upFlg = NULL;
        if (upEntry != NULL)
            upFlg = strstr(upEntry, "\t") + 1;
        
        if (upFlg != NULL && upFlg[0] == 'R') {

            curr = strtok(NULL, "\n");
            continue;
        }

        if (strstr(sMan, p) == NULL) {

            char* flg = strsep(&curr, "\t");
            char* v = strsep(&curr, "\t");
            char* hsh = strsep(&curr, "\t");


            strcat(newMan, "\n");
            strcat(newMan, p);
            strcat(newMan, "\t");
            strcat(newMan, flg);
            strcat(newMan, "\t");
            strcat(newMan, v);
            strcat(newMan, "\t");
            strcat(newMan, hsh);
        }

        curr = strtok(NULL, "\n");
    }



    write(sockfd, upBuff, strlen(upBuff));


    int len = findBytes(sockfd);
    char* compressedFiles = readSockwithLen(sockfd, len);


    if (strstr(compressedFiles, "Error:") != NULL) {

        fprintf(stderr, compressedFiles, strlen(compressedFiles));
        free(cMan);
        free(upBuff);
        return -1; //exit(-1);

    }

    

    char* uCurr = strtok(upBuff, "\n");
    uCurr = strtok(NULL, "\n");
    while (uCurr != NULL) {
        char* p = strsep(&uCurr, "\t");
      //  printf("p: %s\n", p);

        remove(p);
        uCurr = strtok(NULL, "\n");
    }



    int _man = open(mani, O_RDWR | O_TRUNC | O_CREAT, 00600);

    int writeIn = 0;
    int status = 0;
    do { 
        status = write(_man, newMan + writeIn, strlen(newMan) - writeIn);
        if (status < 0 && writeIn == 0) {
         //   fprintf(stderr, "Fatal Error: could not write to file\n");
            close(_man);
            close(sockfd);
            free(cMan);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < strlen(newMan));

    close(_man);

    int tempfd = open("temp.tar.gz", O_RDWR | O_TRUNC| O_CREAT, 00600);

    writeIn = 0;
    status = 0;
    do { 
        status = write(tempfd, compressedFiles + writeIn, len - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(tempfd);
            close(sockfd);
            free(cMan);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < len);

    close(tempfd);



    // untar files and done

    char untar[strlen("tar xzf temp.tar.gz -C ./") + strlen(proj) + 1];
    untar[strlen("tar xzf temp.tar.gz -C ./") + strlen(proj)] = '\0';
    strcpy(untar, "tar xzf temp.tar.gz -C ./");
    strcat(untar, proj);


    if (system(untar) < 0) {

        fprintf(stderr, "Error: failed to untar file contents to %s\n", proj);
        free(cMan);
        free(upBuff);
        close(sockfd);
        remove(upFile);
        remove("temp.tar.gz");
        return -1; //exit(-1);

    }


    remove("temp.tar.gz");
    free(cMan);
    remove(upFile);
    free(upBuff);
    close(sockfd);

    printf("Successful upgrade\n");

    return 1;
}




int commit(char* proj) {

    char update[(strlen(proj)) + strlen(".Update/") + 1]; //done
    char conf[(strlen(proj)) + strlen(".Conflict/") + 1]; //done
    update[(strlen(proj)) + strlen(".Update/")] = '\0'; //done
    conf[(strlen(proj)) + strlen(".Conflict/")] = '\0'; //done
    strcpy(update, proj);
    strcat(update, "/");
 //   strcat(update, proj);
    strcat(update, ".Update");

    strcpy(conf, proj);
    strcat(conf, "/");
  //  strcat(conf, proj);
    strcat(conf, ".Conflict");

    int fd = open(update, O_RDWR);
    if (fd != -1) {

        off_t fsize = lseek(fd, 0, SEEK_END);
        close(fd);
        if ((int)fsize != 0) {

            fprintf(stderr, "Error: must have empty update file to commit\n");
            return -1; //exit(-1);

        } 
    }

    fd = open(conf, O_RDWR);
    if (fd != -1) {

        fprintf(stderr, "Error: must have no conflicts\n");
        close(fd);
        return -1; //exit(-1);

    }
    
    int sockfd = conn();

    char message[7 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "commit\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    char* sMan = readSock(sockfd);

    if (strstr(sMan, "Error") != NULL) {

        fprintf(stderr, "%s\n", sMan);
        close(sockfd);
        free(sMan);
        return -1; //exit(-1);

    }

    char manifest[(strlen(proj)) + strlen(".Manifest/") + 1];//done
    manifest[(strlen(proj)) + strlen(".Manifest/")] = '\0';//done
    strcpy(manifest, proj);
    strcat(manifest, "/");
  //  strcat(manifest, proj);
    strcat(manifest, ".Manifest");

    char* cMan = fillBuff(manifest);


    char com[(strlen(proj)) + strlen(".Commit/") + 1]; //done
    com[(strlen(proj)) + strlen(".Commit/")] = '\0'; //done
    strcpy(com, proj);
    strcat(com, "/");
  //  strcat(com, proj);
    strcat(com, ".Commit");



    char cCopy[strlen(cMan) + 1];
    cCopy[strlen(cMan)] = '\0';
    strcpy(cCopy, cMan);

    char sCopy[strlen(sMan) + 1];
    sCopy[strlen(sMan)] = '\0';
    strcpy(sCopy, sMan);

    char* cVers = strtok(cCopy, "\n");
    char* sVers = strtok(sCopy, "\n");


    if (strcmp(cVers, sVers) != 0) {

        int n = write(sockfd, "Error: project versions are out of sync\n", strlen("Error: project versions are out of sync\n"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write error message to socket\n");
        }
        fprintf(stderr, "Error: update local project\n");
        close(sockfd);
        free(cMan);
        //remove(com);
        return -1; //exit(-1);

    }

    // create commit file


    char comStr[strlen(cMan) + 1];
    bzero(comStr, strlen(cMan) + 1);
    comStr[strlen(cMan)] = '\0';

    strcpy(comStr, cVers);

    char* curr = strtok(cMan, "\n");
    curr = strtok(NULL, "\n");

    //printf("Client Manifest: '%s'\n\n", cMan);

    while (curr != NULL) {

        //printf("curr: '%s'\n\n", curr);

        char* flag = strstr(curr, "\t");
        //printf("\tIN\n\n");

        if (flag[1] == 'I') {

            curr = strtok(NULL, "\n");
            continue;
        }
        else if (flag[1] == 'A') {

            char* p = strsep(&curr, "\t");

            if (strstr(sMan, p) != NULL) {

                fprintf(stderr, "Error: you must update your project files for a successful commit\n");
                write(sockfd, "Error: ", strlen("Error: "));
                free(cMan);
                close(sockfd);
                return -1; //exit(-1);

            }

            int tmp = open(p, O_RDONLY);
            if (tmp < 0) {

                fprintf(stderr, "Error: '%s' is in the manifest, but not at this filepath\n", p);
                write(sockfd, "Error: ", strlen("Error: "));
                free(cMan);
                close(sockfd);
                return -1; //exit(-1);

            }

            char* flg = strsep(&curr, "\t");
            char* v = strsep(&curr, "\t");
            char* hsh = strsep(&curr, "\t");
            
            char* buff = fillBuff(p);
            size_t length = strlen(buff);
            unsigned char _hash[20];
            unsigned char __hash[20];

            SHA1(buff, length, __hash);
            convert(_hash, __hash);


            char hash[21];
            memcpy(hash, _hash, 20);
            hash[20] = '\0';
            
            

            strcat(comStr, "\n");
            strcat(comStr, p);
            strcat(comStr, "\t");
            strcat(comStr, flg);
            strcat(comStr, "\t");
            strcat(comStr, v);
            strcat(comStr, "\t");
            strcat(comStr, hash);

            printf("A %s\n", p);

            free(buff);

        }
        else if (flag[1] == 'S') { 

            char* p = strsep(&curr, "\t");

            char* MorS = "S";

            if (strstr(sMan, p) == NULL) {

                fprintf(stderr, "Error: you must update your project files for a successful commit\n");
                write(sockfd, "Error: ", strlen("Error: "));
                free(cMan);
                close(sockfd);
                return -1; //exit(-1);

            }

            char* flg = strsep(&curr, "\t");
            char* v = strsep(&curr, "\t");
            char* hsh = strsep(&curr, "\t");

            int tmp = open(p, O_RDONLY);
            if (tmp < 0) {

                fprintf(stderr, "Error: '%s' is in the manifest, but not at this filepath\n", p);
                write(sockfd, "Error: ", strlen("Error: "));
                free(cMan);
                close(sockfd);
                return -1; //exit(-1);

            }
            close(tmp);


            char* buff = fillBuff(p);
            size_t length = strlen(buff);
            unsigned char _hash[20];
            unsigned char __hash[20];

            SHA1(buff, length, __hash);
            convert(_hash, __hash);

            char hash[21];

            memcpy(hash, _hash, 20);
           hash[20] = '\0'; //printf("Live hash: %s\n\n", hash);


            char tempS[strlen(sMan) + 1];
            tempS[strlen(sMan)] = '\0';
            strcpy(tempS, sMan);



            char* sLine = strstr(tempS, p);
            //printf("sLine: %s\n", sLine);


            char* _p = strsep(&sLine, "\t");
            char* _flg = strsep(&sLine, "\t");
            char* _vers = strsep(&sLine, "\t");
            char* sHash = strsep(&sLine, "\n");

        
            int cVersion = atoi(v);
            int sVersion = atoi(_vers);

        //    printf("sHash = '%s'\n", sHash);

        //    printf("sVers = %d\tcVers = %d\n", strlen(hash), strlen(sHash));

           
            if (sVersion <= cVersion) {
                
                if (strcmp(sHash, hash) == 0)
                    MorS = "S";
                else    
                    MorS = "M";
            }
            else {

                if (strcmp(sHash, hash) == 0)
                    MorS = "S";
                else {

                    fprintf(stderr, "Error: file versions out of sync, update required for successful commit\n");
                    write(sockfd, "Error: ", strlen("Error: "));
                    free(cMan);
                    close(sockfd);
                    return -1; //exit(-1);

                }

            }
    
            
            strcat(comStr, "\n");
            strcat(comStr, p);
            strcat(comStr, "\t");
            strcat(comStr, MorS);
            strcat(comStr, "\t");
            strcat(comStr, _vers);
            strcat(comStr, "\t");
            strcat(comStr, hash);
            

            free(buff);

            if(strcmp(MorS, "M")== 0){
                 printf("%c %s\n", MorS[0], p);
            }

                
        }
        else {

            strcat(comStr, "\n");
            strcat(comStr, curr);


            char* p = strsep(&curr, "\t");

            if (flag[1] == 'R')
                printf("D %s\n", p);
        }

        //printf("\tOUT\n\n");
        curr = strtok(NULL, "\n");
    }
    

    int cfd = open(com, O_RDWR | O_TRUNC | O_CREAT, 00600);
 

    int writeIn = 0;
    int status = 0;
    do { 
        status = write(cfd, comStr + writeIn, strlen(comStr) - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(cfd);
            close(sockfd);
            free(cMan);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < strlen(comStr));

    close(cfd);

    
    // write to ^ this filename with 


    int n = write(sockfd, comStr, strlen(comStr));
    if (n < 0) {

        fprintf(stderr, "Fatal Error: unable to write commit to socket\n");
        close(sockfd);
        free(cMan);
        return -1; //exit(-1);

    }



    getMessage(sockfd);
    //come back


    free(cMan);
    free(sMan);
    close(sockfd);

    return 1;
}


int push(char* proj) {

    DIR* project = opendir(proj);
    if (project == NULL) {
        fprintf(stderr, "Error: project doesn't exist for push\n");
        return -1; //exit(-1);

    }
    closedir(project);

    char comm[strlen(proj) + strlen("/") + strlen(".Commit") + 1];//done
    comm[strlen(".Commit")];//done??? only one proj
    strcpy(comm, proj);
    strcat(comm, "/");
 //   strcat(comm, proj);
    strcat(comm, ".Commit");

    int cfd = open(comm, O_RDONLY);
    if (cfd < 0) {

        fprintf(stderr, "Error: .Commit file does not exist\n");
        exit(1);
    }
    close(cfd);

    char* cCom = fillBuff(comm);

    int sockfd = conn();

    char message[5 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "push\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    char* sCom = readSock(sockfd);

    //printf("read first sock\n");

    if (strstr(sCom, "Error") != NULL && strlen(sCom) != strlen(cCom)) {

        fprintf(stderr, sCom, strlen(sCom));
        close(sockfd);
        free(cCom);
        return -1; //exit(-1);

    }

    //Now server expects response

    if (strcmp(cCom, sCom) != 0) {

        int n = write(sockfd, "Error: failed push", strlen("Error: failed push"));
        if (n < 0) {

            fprintf(stderr, "Fatal Error: unable to write to socket\n");
        }
        fprintf(stderr, "Error: commit files out of sync, try commiting again to sync files\n");
        free(cCom);
        close(sockfd);
        return -1; //exit(-1);

    }

    // pushing
    char tempPath[strlen(proj) + strlen("/") + strlen(proj) + 1];
    tempPath[strlen(proj) + strlen("/") + strlen(proj)] ='\0';
    strcpy(tempPath, proj);
    strcat(tempPath, "/");
    strcat(tempPath, proj);

    mkdir(tempPath, 00700);

    char newMan[strlen(tempPath) + strlen("/") + strlen(".Manifest") + 1];//done
    newMan[strlen(tempPath) + strlen("/") + strlen(".Manifest")] = '\0';//done
    strcpy(newMan, tempPath);
    strcat(newMan, "/");
  //  strcat(newMan, proj);
    strcat(newMan, ".Manifest");

    char youngMan[strlen(cCom) + 1];
    bzero(youngMan, strlen(cCom) + 1);
    youngMan[strlen(cCom)] = '\0';

 //printf("before strtok\n");
    char* curr = strtok(cCom, "\n");
    int pVers = atoi(curr);
    pVers++;
    char newVers[6];
    bzero(newVers, 6);
    newVers[5] = '\0';
    sprintf(newVers, "%d", pVers);

    //printf("newVers: '%s'\n", newVers);

    strcpy(youngMan, newVers);
    curr = strtok(NULL, "\n");

    //printf("before loop\n");

    while(curr != NULL) {

        char* flag = strstr(curr, "\t");

        char currCpy[strlen(curr) + 1];
        currCpy[strlen(curr)];
        strcpy(currCpy, curr);
        char* _currCpy = currCpy;
        char* fP = strsep(&_currCpy, "\t");

        if (flag[1] != 'I' && flag[1] != 'R') {

            if (flag[1] == 'M') {

                flag[1] = 'S';

                char* p = strsep(&curr, "\t");
                char* flg = strsep(&curr, "\t");
                char* v = strsep(&curr, "\t");
                char* hsh = strsep(&curr, "\t");
                

                int oV = atoi(v);
                oV++;

                char nV[6];
                bzero(nV, 6);
                nV[5] = '\0';
                sprintf(nV, "%d", oV);


                strcat(youngMan, "\n");
                strcat(youngMan, p);
                strcat(youngMan, "\t");
                strcat(youngMan, flg);
                strcat(youngMan, "\t");
                strcat(youngMan, nV);
                strcat(youngMan, "\t");
                strcat(youngMan, hsh);

            }
            else {

                flag[1] = 'S';
                strcat(youngMan, "\n");
                strcat(youngMan, curr);
            }

            
    // this section up until the bracket is creating the file path and file 

    
            char filepath[strlen(proj) + strlen("/") + strlen(fP) + 1];
            filepath[strlen(proj) + strlen("/") + strlen(fP)] = '\0';
            strcpy(filepath, proj);
            strcat(filepath, "/");
            strcat(filepath, fP);
            
            
            char* fName = strrchr(filepath, '/');
            fName[0] = '\0';
            char justPath[strlen(filepath) + 1];
            justPath[strlen(filepath)] = '\0';
            strcpy(justPath, filepath);
            fName[0] = '/';
            

            char command[strlen("mkdir -p ") + strlen(justPath) + strlen(" && cp ") + strlen(fP) + strlen(" ") + strlen(justPath) + 1];
            command[strlen("mkdir -p ") + strlen(justPath) + strlen(" && cp ") + strlen(fP) + strlen(" ") + strlen(justPath)] = '\0';
            strcpy(command, "mkdir -p ");
            strcat(command, justPath);
            strcat(command, " && cp ");
            strcat(command, fP);
            strcat(command, " ");
            strcat(command, justPath);

            //printf("command: '%s'\n", command);

            

            if (system(command) < 0) {

                fprintf(stderr, "Fatal Error: unable to create file paths, shutting down\n");
            }
        
        }
        
        curr = strtok(NULL, "\n");
    } 

    int nmfd = open(newMan, O_RDWR | O_TRUNC | O_CREAT, 00600);

    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(nmfd, youngMan + writeIn, strlen(youngMan) - writeIn);

        if (status < 0 && writeIn == 0) { 
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(nmfd);
            close(sockfd);
            free(cCom);
            return -1; //exit(-1);

        }
        writeIn += status;
                    
    } while (status > 0 && writeIn < strlen(youngMan));

    close(nmfd);


    // change client manifest


    char oldManifest[strlen(proj) + strlen("/") + strlen(".Manifest") + 1];//done
    oldManifest[strlen(proj) + strlen("/") + strlen(".Manifest")] = '\0';//done
    strcpy(oldManifest, proj);
    strcat(oldManifest, "/");
  //  strcat(oldManifest, proj);
    strcat(oldManifest, ".Manifest");

    int omfd = open(oldManifest, O_RDWR | O_TRUNC, 00600);

    writeIn = 0;
    status = 0;
    do{ 
        status = write(omfd, youngMan + writeIn, strlen(youngMan) - writeIn);

        if (status < 0 && writeIn == 0) { 
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(omfd);
            close(sockfd);
            free(cCom);
            return -1; //exit(-1);

        }
        writeIn += status;
                    
    } while (status > 0 && writeIn < strlen(youngMan));

    close(omfd);
    // change client manifest


    char tarFile[strlen(tempPath) + strlen(".tar.gz") + 1];
    tarFile[strlen(tempPath) + strlen(".tar.gz")] = '\0';
    strcpy(tarFile, tempPath);
    strcat(tarFile, ".tar.gz");


    char tarCommand[strlen("tar czf ") + strlen(tarFile) + strlen(" -C ./") + strlen(proj) + strlen(" ") + strlen(" ./") + strlen(proj) + 1];
    tarCommand[strlen("tar czf ") + strlen(tarFile) + strlen(" -C ./") + strlen(proj) + strlen(" ") + strlen(" ./") + strlen(proj)] = '\0';
    strcpy(tarCommand, "tar czf ");
    strcat(tarCommand, tarFile);
    strcat(tarCommand, " -C ./");
    strcat(tarCommand, proj);
    strcat(tarCommand, " ");
    strcat(tarCommand, "./");
    strcat(tarCommand, proj);

    //printf("tarCommand: '%s'\n\n", tarCommand);

    if (system(tarCommand) < 0) {

        fprintf(stderr, "Fatal Error: unable to push, couldnt tar project");
        close(sockfd);
        free(cCom);
        return -1; //exit(-1);

    }

    char* finish = fillTar(tarFile);

    int fTemp = open(tarFile, O_RDONLY);
    off_t fsize = lseek(fTemp, 0, SEEK_END);
    close(fTemp);


    if (write(sockfd, finish, (int)fsize) < 0) {

        fprintf(stderr, "Fatal Error: unable to write to socket\n");
        free(cCom);
        return -1; //exit(-1);

    }


    recPrac(tempPath, remove);
    remove(comm);
    remove(tarFile);



    getMessage(sockfd);

    
    free(cCom);
    close(sockfd);

    return 1;
}


int create(char* proj) {

    int sockfd = conn();

    char message[7 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "create\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }


    //read from socket and check what was read was not an error
    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 80)  {                        // find more elegant solution
       ioctl(sockfd, FIONREAD, &arrlen); // wait every 1 second for 10 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 80 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(sockfd, buff+readIn, arrlen - readIn); 
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
                close(sockfd);
                return -1; //exit(-1);

            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

//                                                         find out how to do a substring
    if (strstr(buff, "Error") != NULL) {
        printf("%s\n", buff);
    }
    else {

        mkdir(proj, 00700);

        char manifest[strlen(proj) + strlen("/")  + strlen(".Manifest") + 1];    //  length of "./" + ".Manifest + length of proj"//done
        manifest[strlen(proj) + strlen("/") + strlen(".Manifest")] = '\0';//done
        strcpy(manifest, proj);
        strcat(manifest, "/");
  //      strcat(manifest, proj);
        strcat(manifest, ".Manifest");

        int fd = open(manifest, O_RDWR | O_TRUNC | O_CREAT, 00600);
        
        int writeIn = 0;
        int status = 0;
        do{ 
            status = write(fd, buff + writeIn, strlen(buff) - writeIn);

            if (status < 0 && writeIn == 0) { 
                fprintf(stderr, "Fatal Error: could not write to file\n");
                close(fd);
                close(sockfd);
                return -1; //exit(-1);

            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(buff));

        close(fd);

        printf("Successfully created project\n");
    }

    close(sockfd);

    return 1;
}


int destroy(char* proj) {

    int sockfd = conn();

    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "destroy\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    getMessage(sockfd);

    close(sockfd);

    return 1;
}


int add(char* proj, char* fName) {

    DIR* project = opendir(proj);
    if (project == NULL) {
        fprintf(stderr, "Fatal Error: project doesn't exist\n");
        return -1; //exit(-1);

    }
    closedir(project);


    char fullName[strlen(fName) + strlen(proj) + 2];
    fullName[strlen(fName) + strlen(proj)] = '\0';
    strcpy(fullName, proj);
    strcat(fullName, "/");
    strcat(fullName, fName);

    int fd = open(fullName, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Fatal Error: file \"%s\" does not exist in project directory\n", fullName);
        return -1; //exit(-1);

    }
    close(fd);
    fd = 0;

    char* file = fillBuff(fullName);
    size_t length = strlen(file);
    unsigned char _hash[20];
    unsigned char __hash[20];

    SHA1(file, length, __hash);
    int i =0;
    //this is add

    convert(_hash,__hash);
 

    char hash[21];
    memcpy(hash, _hash, 20);

    hash[20] = '\0';


    char manifest[2 + (strlen(proj)) + strlen(".Manifest")];     //done             // could be we need 1 additional for null character and have to set it with next line
    manifest[1 + (strlen(proj)) + strlen(".Manifest")] = '\0';//done
    strcpy(manifest, proj);
    strcat(manifest, "/");
 //   strcat(manifest, proj);
    strcat(manifest, ".Manifest");

    fd = open(manifest, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Fatal Error: unable to open manifest file in project directory\n");
        return -1; //exit(-1);

    }
    close(fd);

    char* oldMan = fillBuff(manifest);

    char* element = strstr(oldMan, fullName);

    if (element == NULL) {

        char addNew[strlen(fullName) + 24 + 4]; //length of hash = 20, 1 null character, '\n', version and identifier <A,S,M,R> + 4 tabs
        addNew[27 + strlen(fullName)] = '\0';
        char newMan[strlen(addNew) + strlen(oldMan) + 1];
        newMan[strlen(addNew) + strlen(oldMan)] = '\0';


        strcpy(addNew, "\n");
        strcat(addNew, fullName);
        strcat(addNew, "\t");
        strcat(addNew, "A");
        strcat(addNew, "\t");
        strcat(addNew, "0");
        strcat(addNew, "\t");
        strcat(addNew, hash);

        strcpy(newMan, oldMan);
        strcat(newMan, addNew);

        fd = open(manifest, O_RDWR | O_TRUNC);
        int writeIn = 0;
        int status = 0;
        do{ 
            status = write(fd, newMan + writeIn, strlen(newMan) - writeIn);

            if (status < 0 && writeIn == 0) { 
                fprintf(stderr, "Fatal Error: could not write to file\n");
                free(oldMan);
                close(fd);
                return -1; //exit(-1);

            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(newMan));

        close(fd);

    }
    else {

        char* flag = strstr(element, "\t");
        if (flag[1] == 'R') {

            flag[1] = 'S';
        }
        else if (flag[1] == 'I') {

            flag[1] = 'A';
        }
        else {

            fprintf(stderr, "Error: file already exists in project manifest\n");
            free(oldMan);
            return -1; //exit(-1);

        }


        int nfd = open(manifest, O_RDWR | O_TRUNC);
        int writeIn = 0;
        int status = 0;
        do{ 
            status = write(nfd, oldMan + writeIn, strlen(oldMan) - writeIn);

            if (status < 0 && writeIn == 0) { 
                fprintf(stderr, "Fatal Error: could not write to file\n");
                free(oldMan);
                close(nfd);
                return -1; //exit(-1);

            }
            writeIn += status;
                        
        } while (status > 0 && writeIn < strlen(oldMan));

        close(nfd);

    }

    printf("Successfully added file to project manifest\n");

    free(oldMan);
    return 1;
}


int rem(char* proj, char* fName) {

    DIR* project = opendir(proj);
    if (project == NULL) {
        fprintf(stderr, "Fatal Error: project doesn't exist\n");
        return -1; //exit(-1);

    }
    closedir(project);

    char manifest[2 + (strlen(proj)) + strlen(".Manifest")];    //done              // could be we need 1 additional for null character and have to set it with next line
    manifest[1 + (strlen(proj)) + strlen(".Manifest")] = '\0';//done
    strcpy(manifest, proj);
    strcat(manifest, "/");
  //  strcat(manifest, proj);
    strcat(manifest, ".Manifest");

    int fd = open(manifest, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Fatal Error: unable to open manifest file in project directory\n");
        return -1; //exit(-1);

    }
    close(fd);

    char* mani = fillBuff(manifest);
    char* name = strstr(mani, fName);
    if (name == NULL) {

        fprintf(stderr, "Error: file already removed or was never added in manifest\n");
        free(mani);
        return -1; //exit(-1);

    }

    char* flag = strstr(name, "\t");

    if (flag[1] == 'A') {

        flag[1] = 'I';
    }
    else if (flag[1] == 'S') {
        
        flag[1] = 'R';
    }
    else {
        
        fprintf(stderr, "Error: file already removed or was never added in manifest\n");
        free(mani);
        return -1; //exit(-1);

    }


    fd = open(manifest, O_RDWR | O_TRUNC);
    int writeIn = 0;
    int status = 0;

    do{ 
        status = write(fd, mani + writeIn, strlen(mani) - writeIn);

        if (status < 0 && writeIn == 0) { 
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            free(mani);
            return -1; //exit(-1);

        }
        writeIn += status;
                    
    } while (status > 0 && writeIn < strlen(mani));

    close(fd);


    printf("Succussfully removed file from manifest\n");
    free(mani);
    return 1;
}


int currentversion(char* proj) {

    int sockfd = conn();

    char message[15 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "currentversion\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    //printf("manifest:\n%s\n", fillBuff("./testProj/testProj.Manifest"));
    char* retMes = readSock(sockfd);
    printf("%s", retMes);

    free(retMes);
    close(sockfd);

    return 1;
}


int history(char* proj) {

    int sockfd = conn();
    remove(",History");
    char message[8 + strlen(proj)]; // integer = length of command + tab escape char
    strcpy(message, "history\t");
    strcat(message, proj);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    char* ret = readSock(sockfd);
    
    if (strstr(ret, "Error") != NULL) {

        fprintf(stderr, "%s\n", ret);
        close(sockfd);
        free(ret);
        return -1; //exit(-1);

    }
/*
    char hist[ strlen(".History/") + 1];//done removed strlen(proj)
    hist[strlen(".History/")] = '\0';//done
 //   strcpy(hist, proj);
 //   strcat(hist, "/");
  //  strcat(hist, proj);
    strcat(hist, ".History");
*/
    char hist[] = ".History";
    int fd = open(hist, O_RDWR | O_TRUNC | O_CREAT, 00600);
    if (fd < 0) {

        fprintf(stderr, "Error: unable to open history file for write on client\n");
        close(sockfd);
        free(ret);
        return -1; //exit(-1);

    }

    int writeIn = 0;
    int status = 0;
    do{ 
        status = write(fd, ret + writeIn, strlen(ret) - writeIn);
        if (status < 0 && writeIn == 0) {
            fprintf(stderr, "Fatal Error: could not write to file\n");
            close(fd);
            close(sockfd);
            free(ret);
            return -1; //exit(-1);

        }
        writeIn += status;
                     
    } while (status > 0 && writeIn < strlen(ret));


    close(fd);
    
    char* printbuff = fillBuff(hist);
    printf("%s\n\n", printbuff);
    free(printbuff);

    printf("Successfully transferred history file to client\n");

    close(sockfd);

    return 1;
}


int rollback(char* proj, char* vers) {

    int sockfd = conn();

    char message[10 + strlen(proj) + strlen(vers)]; // integer = length of command + tab escape char + another tab escape char after proj
    strcpy(message, "rollback\t");
    strcat(message, proj);
    strcat(message, "\t");
    strcat(message, vers);

    if (write(sockfd, message, strlen(message)) < 0) {

        fprintf(stderr, "Fatal Error: could not write to socket\n");
        close(sockfd);
        return -1; //exit(-1);

    }

    getMessage(sockfd);


    close(sockfd);

    return 1;
}


#endif