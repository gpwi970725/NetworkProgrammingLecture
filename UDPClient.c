#include "headerFiles.h"
#define BUFLEN 512
#define PORT 9930 /* port No for daytime : 13 */

void err(char *s) {
	perror(s);
	exit(1);
}

int main(int argc, char** argv) {
	struct sockaddr_in serv_addr;
	int sockfd, i, slen=sizeof(serv_addr);
	char buf[BUFLEN];

	if(argc!= 2) {
		printf("Usage : %s <Server-IP>\n",argv[0]);
	exit(0);
	}

	if ((sockfd= socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err("socket");
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (inet_aton(argv[1], &serv_addr.sin_addr) == 0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	while(1) {
		printf("\nEnterdata to send(Type exit and press enter to exit) : ");
		scanf("%[^\n]",buf);
		getchar();
		if(strcmp(buf,"exit") == 0)
			exit(0);
		if (sendto(sockfd, buf, BUFLEN, 0, (struct sockaddr*) &serv_addr, slen) == -1)
			err("sendto()");
		// recvfrom/¡¦.
	}
	close(sockfd);
	return 0;
}
