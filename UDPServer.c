#include "headerFiles.h"
#define BUFLEN 512
#define PORT 9930	/*port no. of daytime: 13 */

void err(char*str) {
	perror(str);
	exit(1);
}

int main(void) {
	struct sockaddr_in my_addr, cli_addr;
	int sockfd, i;
	socklen_t slen = sizeof(cli_addr);
	char buf[BUFLEN];
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		err("socket");
	else
		printf("Server : Socket() successful\n");

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr* ) &my_addr, sizeof(my_addr)) == -1)
		err("bind");
	else
		printf("Server : bind() successful\n");

	while(1) {
		if(recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*)&cli_addr, &slen)==-1)
			err("recvfrom()");
			printf("Received packet from %s:%d\nData: %s\n\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buf);
	}

	close(sockfd);
	return 0;
}
