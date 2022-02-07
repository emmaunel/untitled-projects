#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void handler(int signal) {
    if (signal == SIGINT) {
        printf("Can't escape me !!!\n$ ");
    } else if (signal == SIGTSTP) {
        printf("We don't do that here!!!\n");
    } else if (signal == SIGTRAP) {
        printf("Capture all pokemon\n");
    }
}

int main (){
    setuid(0); // making sure the process is run by root, maybe
    char user_input[1000];
    printf("Welcome to the trolling shell\n");
    signal(SIGINT, handler);
    signal(SIGTSTP, handler);
    signal(SIGTRAP, handler);
    while(1) {
        printf("$ ");
        scanf("%s", user_input);
        if (strcmp(user_input, "bingus") == 0 ) {
            printf("[+] Hello bingus\n");
            printf("[+] Getting you a bash shell\n");
            char *args[] = {"/bin/bash", NULL};
            execvp(args[0], args);
        }  
        
        if (strcmp(user_input, "exit") == 0) {
            printf("Thanks for using me. See ya next time <3\n");
            exit(0);
        }

        // Trolling here
        printf("Sorry, we be trolling sometimes.....\n");

    }
}