#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


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

void *execString(void *s) {
    char *args[100];
    parseArgs((char*)s, args);

    int status;
    int pid = fork();
    if (pid==0) execvp(args[0], args);
    else waitpid(pid, &status, 0);
    free(s);
}

int main()
{
    char *args[100];
    char s[100];
    char *tmp;
    pthread_t tid;

    while (true) {
        printf(">");
        fgets(s, sizeof(s), stdin);
        fflush(stdout);

        if (!strcmp(s, "quit")) {
            printf("User quit\n");
            return 0;
        }

        tmp = malloc(100);
        strcpy(tmp, s);
        pthread_create(&tid, NULL, execString, (void*)tmp);
        pthread_join(tid, NULL);
    }

	return 0;
}
