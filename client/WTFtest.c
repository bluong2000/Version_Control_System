#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>


void * hello(void * arg){

  char hostname[1024];
  gethostname(hostname, sizeof(hostname)-1);
//  printf("hostname:%s\n", hostname);
  //gets the host name and uses configure
 // char command1[] ="cd client && ./WTF configure  frost.cs.rutgers.edu 9075";
  char command1[strlen(hostname)+ strlen("cd client && ./WTF configure  9075")+1];
  command1[strlen(hostname)+ strlen("cd client && ./WTF configure  9075")] = '\0';

  strcpy(command1, "cd client  && ./WTF configure ");
  strcat(command1, hostname);
  strcat( command1, " 9077");
  printf("command:%s\n", command1);
  system(command1);

  char command2[] = "cd client && ./WTF create test";
  system(command2);
 // system("cd client");


  char command3[] = "cd client && ./WTF destroy test";
  system(command3);

 
  char command4[] = "cd client && ./WTF create test";
  system(command4);


//  char command5[] = "mkdir client/test/";
  //system(command5);

 

  char command6[] = "mkdir client/test/sub1";
  system(command6);

   char command7[] = "mkdir client/test/sub2";
  system(command7);

  char command8[] = "mkdir client/test/sub1/sub3";
  system(command8);

  char command9[] = " echo \"hello\" >> client/test/file1.txt ";
  system(command9);

  char command10[] = " echo \"hello\" >> client/test/sub1/file2.txt ";
  system(command10);

  char command11[] = " echo \"\" >> client/test/sub1/file3.txt ";
  system(command11);

  char command12[] = " echo \"This proj was v hard\" >> client/test/sub2/file4.txt ";
  system(command12);

  char command13[] = " echo \"This is file 5\" >> client/test/sub1/sub3/file5.txt ";
  system(command13);

  //testing add command
  char command14[] = " cd client && ./WTF add test sub1/sub3/file5.txt";
  system(command14);  

  //
  char command15[] = " cd client && ./WTF add test sub2/file4.txt ";
  system(command15);  

  char command16[] = " cd client && ./WTF add test sub1/file3.txt";
  system(command16);  

  char command17[] = " cd client && ./WTF add test sub1/file2.txt ";
  system(command17);  

  char command18[] = " cd client && ./WTF add test file1.txt ";
  system(command18);

  //testing remove 

  char command19[] = " cd client && ./WTF remove test file1.txt ";
  system(command19);

  //testing commit

  char command20[] = " cd client && ./WTF commit test  ";
  system(command20);

  //test push
  char command21[] = " cd client && ./WTF push test  ";
  system(command21);

  //try editing a file committing and pushing again
  char command22[] = " echo \"This file was edited\" >> client/test/sub1/sub3/file5.txt ";
  system(command22);

  char command23[] = " cd client && ./WTF commit test  ";
  system(command23);

   char command24[] = " cd client && ./WTF push test  ";
  system(command24);

//current version
   char command25[] = " cd client && ./WTF currentversion test  ";
  system(command25);

// check update
   char command26[] = " cd client && ./WTF update test  ";
  system(command26);


// check roll back
  char command27[] = " cd client && ./WTF rollback test 1";
  system(command27);


// test upgrade now that repo has been edited
   char command28[] = " cd client && ./WTF update test  ";
  system(command28);

  char command29[] = " cd client && ./WTF upgrade test  ";
  system(command29);

// test checkout deletes client's proj first

  system("cd client && rm -r test ");

  char command30[] = " cd client && ./WTF checkout test  ";
  system(command30);

//test history
  system("cd client && ./WTF history test");


  

  return 0;

}

int main(void) {
  
 
  pthread_t tid;
  pthread_create(&tid, NULL, hello, NULL);
  char command1[] = "cd server && ./WTFserver 9077";
  system(command1);
  

  pthread_join(tid, NULL);
 

 
  


  return 0;
}