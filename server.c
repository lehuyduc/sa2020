#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

char s[100];

int main(int argc, char *argv[]) {
    int sockfd, clientfd;
    struct sockaddr_in saddr;
    unsigned short port = 8784;
    unsigned int clen;
    pthread_t input_thread;

    // create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
		return -1;
	}

    // bind the socket to port
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		printf("Error binding socket\n");
		close(sockfd);
		return -1;
	}

    // then listen
    listen(sockfd, 5);
	clen = sizeof(saddr);


    while (1) {
        // wait and accept an incoming connection
        printf("server> accepting incoming connection\n");
        clientfd = accept(sockfd, (struct sockaddr *) &saddr, &clen);
        // TODO: communicate with connected client here

        int pid = fork();
        if (pid==0) {
            printf("A client connected\n");
            while (1) {
                read(clientfd, s, sizeof(s));
                printf("client send: %s\n",s);

                printf("server>");
                fgets(s, sizeof(s), stdin);
                write(clientfd, s, strlen(s));
            }
            return 0;
        }
        else {
            continue;
        }
    }
    close(clientfd);

}



