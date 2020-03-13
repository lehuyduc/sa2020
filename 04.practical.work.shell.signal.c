#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int childId;

void parseArgs(char *s, char *args[]) {
    int i, j, n, nargs;
    char arg[100];

    n = strlen(s);
    while (s[n-1]==' ' || s[n-1]=='\n') n--;
    s[n] = NULL;

    nargs = 0;
    for (i=0; i<n;i++) {
        j = i;
        while (j<n && s[j]!=' ') j++;
        j--;
        memcpy(arg, &s[i], j-i+1);
        arg[j-i+1] = 0;
        i = j+1;

        nargs++;
        args[nargs-1] = malloc(sizeof(char)*100);
        strcpy(args[nargs-1], arg);
    }
    args[nargs] = NULL;
}

void mainHandler(int signal) {
    printf("Dad receiveed signal %d\n",signal);
    kill(childId, SIGINT);
}

void killDadHandler(int signal) {
    printf("Dad receiveed signal %d\n",signal);
    exit(0);
}

void handler(int signal) {
    printf("Child received signal %d\n",signal);
    switch (signal) {
    case SIGINT:
        printf("Killing shell\n");
        kill (getppid(), SIGINT);
        break;
    }
}

int main()
{
    char *args[100];
    char *s[100];

    childId = fork();
    if (childId==0) signal(SIGINT, handler);
    else {
        signal(SIGTSTP, mainHandler);
        signal(SIGINT, killDadHandler);
    }

    while (true) {
        fgets(s, sizeof(s), stdin);
        printf("user typed: %s",s);
        fflush(stdout);

        if (!strcmp(s, "quit")) {
            printf("User quit\n");
            return 0;
        }

        parseArgs(s, args);

        int pid = fork();
        if (pid==0) execvp(args[0], args);
        else {
            wait();
        }
    }

    return 0;
}
