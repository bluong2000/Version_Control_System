#include "server.h"
pthread_mutex_t lock;

int newsockfd;
int sockfd;

//sockfd = socket(AF_INET, SOCK_STREAM, 0);
 typedef struct Node{
    pthread_t id;
    struct Node *next;
}tnode;

tnode **head;

struct thread_args{
    int newsockfd;
    int sockfd;

};

void init(){
    head = malloc(sizeof(struct Node));

}

void add(tnode **head, pthread_t id){
    tnode *new = (tnode*)malloc(sizeof(tnode));

  //  printf(" in head\n");

    new -> next = (*head);

    new-> id = id;

    (*head) = new;
//    printf("new id: %d\n", new->id);

  //  printf("head id: %d\n", (*head)->id);

}

void deletefirst(tnode ** head){ //done

    if ((*head)->next == NULL){
        pthread_join((*head)->id, NULL);
        printf("thread %d disconnected from server\n" , (*head)->id );

        free(*head);
        *head = NULL;
        return;
    }else{
        pthread_join((*head)->id, NULL);
        printf("thread %d disconnected from server\n" , (*head)->id );

        tnode * temp;
        temp = *head;
        (*head)= (*head)->next;
        free(temp);
        return;
    }
}

void delete(pthread_t id){
    //store head
  //  printf("thread removed:%d\n", id );

    tnode *temp = *head, *prv;
   // printf("id thread removed:%d\n",temp-> id );

    if (temp!= NULL && temp->id ==id){
        
        *head = temp->next;
        free(temp);
        return;
    }
  //  printf("thread ids:%d\n", temp-> id );

    while ( temp!= NULL && temp->id != id){
            prv = temp;
        //    printf("thread in loop ids:%d\n", temp-> id );

            temp = temp->next;


    }
    if(temp ==NULL){
        return;
    }
        pthread_join((temp)->id, NULL); //add join here? or is it main thread?
        printf("free thread removed:%d\n", temp->id );
        prv ->next = temp->next;
        free(temp);
        return;

}





void * clienthandler(){

    //  struct thread_args *arg = (struct thread_args*)args;
  //  newsockfd = (*(int*)args); //dec of newsockfd
   // int sockfd = 3;
  //  printf("client sockfd:%d\n", sockfd );
   // printf("client newsockfd:%d\n", newsockfd );

   // printf("out of loop\n");
    int arrlen = 0;
    int c = 0;
    while (arrlen == 0 && c != 20)  {                        // find more elegant solution
       ioctl(newsockfd, FIONREAD, &arrlen); // wait every 1 second for 20 seconds, after 10 seconds return error
       delay(1);
       c++;
    }

    if (c == 20 && arrlen == 0) {
        fprintf(stderr, "Fatal Error: server unable to send data\n");
        close(newsockfd);
        //close(sockfd);;
        //exit(-1);
    }

    char buff[arrlen + 1];
    bzero(buff,arrlen + 1);
    buff[arrlen] = '\0';
    int readIn = 0;
    int status = 0;

    do{  // reads buffer char by char
            status = read(newsockfd, buff+readIn, arrlen - readIn);
            if (status < 0 && readIn == 0) {
                fprintf(stderr, "Fatal Error: could not read from socket\n");
              //  exit(-1);
            }
            readIn += status;
                     
    } while (status > 0 && readIn < arrlen);

   // printf("\t%s\n", buff);
    //printf("Here is the message: %s\n", buff);


    char* command = strtok(buff, "\t");
    char* proj = strtok(NULL, "\t");

    //printf("Here is the message: %s\t%s\n", comm, proj);


    //lock here
    pthread_mutex_lock(&lock);

    if (strcmp("checkout", command) == 0) {
        
        checkout(sockfd, newsockfd, proj);
    }
    else if (strcmp("update", command) == 0) {

        

        update(sockfd, newsockfd, proj);
     //   printf("update selected\n");

        
    }
    else if (strcmp("upgrade", command) == 0) {

        

        upgrade(sockfd, newsockfd,proj);
        
    }
    else if (strcmp("commit", command) == 0) {

        

        commit(sockfd, newsockfd, proj);

        
    }
    else if (strcmp("push", command) == 0) {

        

        push(sockfd, newsockfd, proj);
        
    }
    else if (strcmp("create", command) == 0) { 

        if (create(newsockfd, proj) == -1) {
            char*retMes = "Error: project already exists in server's repository";
            int n = write(newsockfd, retMes, strlen(retMes));

            if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket");
            //close(sockfd);;
            close(newsockfd);
           // exit(-1);
            } 
        }
  
        
        //printf("sending \"%s\" message size = %d\n", retMes, strlen(retMes));
    }
    else if (strcmp("destroy", command) == 0) {

        char* retMes = "Successfuly deleted project";

        if (destroy(proj) == -1) {
            retMes = "Error: project does not exist in server repository";
        }

        int n = write(newsockfd, retMes, strlen(retMes));

        if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
        //    close(sockfd);
         //   close(newsockfd);
           // exit(-1);
        }  

        
    }
    else if (strcmp("currentversion", command) == 0) {

        char projPath[strlen(proj) + strlen("./repository/") + 2];
        projPath[strlen(proj) + strlen("./repository/") + 1] = '\0';
        strcpy(projPath, "./repository/");
        strcat(projPath, proj);
        strcat(projPath, "/");

        DIR* dd = opendir(projPath);
        if (dd == NULL) {
            int n = write(newsockfd, "Error: project does not exist in server repository\n", strlen("Error: project does not exist in server repository\n"));

            if (n < 0) {

            fprintf(stderr, "Fatal Error: could not write to socket\n");
            //close(sockfd);;
            close(newsockfd);
           // exit(-1);
            }  
        }
        else {
            closedir(dd);
            currentversion(sockfd, newsockfd, proj);
        }
        
    }
    else if (strcmp("history", command) == 0) {
        
        history(sockfd, newsockfd, proj);

    }
    else if (strcmp("rollback", command) == 0) {

        char* version = strtok(NULL, "\t");

        rollback(sockfd, newsockfd, proj, version);
        
    }
    else {

        fprintf(stderr, "Fatal Error: \"%s\" is not a valid command, please refer to the readme for valid commands\n", command);
      //  exit(-1);
    }

    //unlock
    pthread_mutex_unlock(&lock);

    pthread_t num = pthread_self();
  //  printf("pthread self : %d\n", num);
    delete(num);


    //printf("got here to close sockets\n");
  //  close(sockfd);;;
   // close(newsockfd);  //might need to close later
    return ;
}

void inthandler(int signal){
    exit(0);
    
}


void terminate(){
   // printf("in terminate\n");
    while(  *head != NULL){
    //    printf("head id: %d\n", (*head)-> id);
        deletefirst(head); //this line  could be wrong

      
    }
     close(newsockfd);
     pthread_mutex_destroy(&lock);

  //  printf("joining threads\n");
      printf("\nServer stopped\n");

    return;
}

int main (int argc,  char *argv[]) {
   // tnode *head = NULL;
    init();
    signal(SIGINT, inthandler);
    atexit(terminate);

    if (argc < 2 || atoi(argv[1]) < 5000 || atoi(argv[1]) > 65000) {

        fprintf(stderr, "Fatal Error: Please enter a valid port number greater than 5000 and less than 65000\n");
        exit(-1);
      // return -1;
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // may need to add an error check
    if (sockfd < 0) {

        fprintf(stderr, "Fatal Error: could not make socket\n");
        exit(-1);
    }
    struct sockaddr_in serverAddressInfo;
    struct sockaddr_in clientAddressInfo;
    int clilen;
    newsockfd;
    int n;

    bzero((char *) &serverAddressInfo, sizeof(serverAddressInfo));
    serverAddressInfo.sin_port = htons(atoi(argv[1])); 
    serverAddressInfo.sin_family = AF_INET;
    serverAddressInfo.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serverAddressInfo, sizeof(serverAddressInfo)) < 0) {
        fprintf(stderr,"Fatal Error: could not bind\n");
        exit(-1);
    }

 
    listen(sockfd, 0);

    clilen = sizeof(clientAddressInfo);
   //s printf("establishing connection\n");
  


    int *arg = malloc(sizeof(*arg));
    *arg = newsockfd;
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);
        if (newsockfd < 0) {
            fprintf(stderr, "Fatal Error: Couldn't accept request\n");
            //exit(-1);
            continue;
        }
        printf("accepted client connection to server\n");

        pthread_t threadid;
  //      printf("threadid:%d \n", threadid);
   //     printf("threadid:%d \n", &threadid);


        if( pthread_create(&threadid, NULL, clienthandler, NULL) != 0) {
                printf(" couldn't create thread\n");

        }
        add(head , threadid);

      //  pthread_join(&threadid, NULL);



        
    }

     //   printf("loop interate\n");
    
//thread
/*
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &clientAddressInfo, &clilen);

        //client has tried to connect
        if (newsockfd < 0) {
            fprintf(stderr, "Fatal Error: Couldn't accept request from client\n");
            // close(sockfd);
           // exit(-1);
           continue;
        }
        //my code
        char smess[20] = "reached server";
        pthread_t id;
        if(pthread_create(&id, NULL, threadfun, &newsockfd ) != 0 ){
            char* fail = "thread failed";
            send(newsockfd, fail, sizeof(fail), 0);
        }
        //thread did not fail
        send(newsockfd, smess, sizeof(smess), 0 );
        add(&threadLL, id);


    }
*/

    return 0;
}