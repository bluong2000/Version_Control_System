#include "client.h"




int main (int argc,  char *argv[]) {

    if (argc < 3) {

        fprintf(stderr, "Fatal Error: too few arguments, enter one of the valid commands present in the readme\n");
        exit(-1);
    }
    else if (argc > 4) {

        fprintf(stderr, "Fatal Error: too many arguments, enter one of the valid commands present in the readme\n");
        exit(-1);
    }

    char* command = argv[1];



    if (strcmp("configure", command) == 0) {

        if (argc != 4) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        configure(argv[2], argv[3]);
    }
    else if (strcmp("checkout", command) == 0) {
        
        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
            
        }

        DIR* pro = opendir(argv[2]);
        if (pro != NULL) {

            fprintf(stderr, "Error: client already has project file\n", command);
            closedir(pro);
            exit(-1);
        }
        
        checkout(argv[2]);
         printf("Client disconnected from server\n");
   
    }
    else if (strcmp("update", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        update(argv[2]);
         printf("Client disconnected from server\n");
   
    }
    else if (strcmp("upgrade", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        upgrade(argv[2]);
          printf("Client disconnected from server\n");

        
    }
    else if (strcmp("commit", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        commit(argv[2]);
        printf("Client disconnected from server\n");

        
    }
    else if (strcmp("push", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        push(argv[2]);
        printf("Client disconnected from server\n");

        
    }
    else if (strcmp("create", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        create(argv[2]);
        printf("Client disconnected from server\n");

    }
    else if (strcmp("destroy", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        destroy(argv[2]);
        printf("Client disconnected from server\n");

        
    }
    else if (strcmp("add", command) == 0) {

        if (argc != 4) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        add(argv[2], argv[3]);
        
    }
    else if (strcmp("remove", command) == 0) {

        if (argc != 4) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        rem(argv[2], argv[3]);

    }
    else if (strcmp("currentversion", command) == 0) {

        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        currentversion(argv[2]);
        printf("Client disconnected from server\n");

        
    }
    else if (strcmp("history", command) == 0) {
        
        if (argc != 3) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        history(argv[2]);
      printf("Client disconnected from server\n");

    }
    else if (strcmp("rollback", command) == 0) {

        if (argc != 4) {

            fprintf(stderr, "Fatal Error: invalid arguments for %s, refer to the readme for the correct parameters for the configure command\n", command);
            exit(-1);
        }

        rollback(argv[2], argv[3]);
         printf("Client disconnected from server\n");
   
    }
    else {

        fprintf(stderr, "Fatal Error: \"%s\" is not a valid command, please refer to the readme for valid commands\n", command);
        exit(-1);
    }


    return 0;
}