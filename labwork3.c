#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

void parseArgs(char *s, char *args[]) {
    int i, j, n, nargs;
    char arg[100];

    n = strlen(s);
    nargs = 0;
    for (i=0; i<n;i++) {
        j = i;
        while (j<n && s[j]!=' ') j++;
        if (j==n) j--;
        memcpy(arg, &s[i], j-i+1);
        i = j;

        nargs++;
        args[nargs-1] = malloc(sizeof(char)*100);
        strcpy(args[nargs-1], arg);
    }
    args[nargs] = NULL;
}

int main()
{
    char *args[100];
    char *s[100];

    while (true) {
        scanf("%s",s);
        printf("user typed: %s\n",s);
        fflush(stdout);
        if (!strcmp(s, "quit")) {
            printf("User quit\n");
            return 0;
        }

        parseArgs(s, args);

        int pid = fork();
        if (pid==0) execvp(args[0], args);
    }

	return 0;
}
