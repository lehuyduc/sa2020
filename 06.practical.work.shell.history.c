#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

bool exiting = false;
int childId;
char *args[100];
char *s[100];

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

void dadHandler(int signal) {
    printf("Dad receiveed signal %d\n",signal);
    kill(childId, SIGINT);
}

void killDadHandler(int signal) {
    printf("Dad receiveed signal %d\n",signal);
    exit(0);
}

void childHandler(int signal) {
    printf("Child received signal %d\n",signal);
    switch (signal) {
    case SIGINT:
        printf("Killing shell\n");
        kill (getppid(), SIGINT);
        exit(0);
    }
}

void updateHistory() {
    FILE *file = fopen("command.log", "a");
    fwrite(s, 1, strlen(s), file);
    fclose(file);
}

void printHistory() {
    File *file = fopen("command.log", "r");
    if (file==NULL) {printf("No history\n"; return;}
    while (getline(s, strlen(s), file)) {
        printf("%s",s);
    }
    fclose(file);
}

int main()
{
    childId = fork();
    if (childId==0) {
        signal(SIGINT, childHandler);
        while (1) {}
    }
    else {
        signal(SIGTSTP, dadHandler);
        signal(SIGINT, killDadHandler);
    }

    exiting = false;
    while (!exiting) {
        fgets(s, sizeof(s), stdin);
        printf("user typed: %s",s);
        fflush(stdout);

        if (!strcmp(s, "quit")) {
            printf("User quit\n");
            return 0;
        }

        if (!exiting) {
            parseArgs(s, args);

            int pid = fork();
            if (pid==0) execvp(args[0], args);
            else {
                wait();
                updateFile();
            }
        }
    }

    printHistory();
    return 0;
}
