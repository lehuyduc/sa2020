#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    struct sockaddr_in testaddr[10];
    struct sockaddr_in saddr;
    struct hostent *h;
    int sockfd;
    unsigned int port = 8784;
    char s[100];

    if (argc > 1) {
        memcpy(s, argv[1], strlen(argv[1]) + 1);
    }
    else {
        printf("Enter hostname: ");
        scanf("%s", s);
    }

    // create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Error creating socket\n");
		return -1;
	}

    // resolve host address
    if ((h = gethostbyname(s)) == NULL) {
		printf("Unknown host\n");
		close(sockfd);
		return -1;
	}

    // init sockaddr
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr = *(struct in_addr *)h->h_addr_list[0];
    saddr.sin_port = htons(port);

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		printf("Cannot connect to host\n");
		close(sockfd);
		return -1;
	}

	printf("Connected successfully!\n");

    while (1) {
        // TODO: add logics to communicate with server
        printf("client>");
        fgets(s, sizeof(s), stdin);
        write(sockfd, s, strlen(s));

        read(sockfd, s, sizeof(s));
        printf("server send: %s\n",s);
    }
    close(sockfd);
}
