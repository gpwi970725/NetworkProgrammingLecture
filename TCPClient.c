#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXBUF 1024

int main(int argc, char *argv[]) { // char ** argv
	int server_sockfd, client_sockfd;
	int state, client_len;
	int pid;

	struct sockaddr_in clientaddr, serveraddr;

	char buf[MAXBUF];

	if (argc!= 2) {
		printf("Usage : %s [port]\n", argv[0]);
		return 1;
	}

	// TCP 소켓을만듦
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}

	// 서버에서받아들인클라이언트주소와포트주소를설정
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	// 설정된주소를소켓에연결
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (state == -1) { // 리턴값의상태확인
		perror("bind error : ");
		exit(0);
	}

	// 수신할수있는클라이언트연결수가5개로설정
	state = listen(server_sockfd, 5);
	if (state == -1) { // 리턴값의상태확인
		perror("listen error : ");
		exit(0);
	}

	while(1) {
		// 클라이언트의연결을받아들임
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
		if (client_sockfd == -1) { // 항상오류는검사해주어야함
			perror("Accept error : ");
			exit(0);
		}
		// 클라이언트와연결이성공적이면자식프로세스를하나만듦
		pid = fork();
		if (pid== 0) { // 자식프로세스인경우다음문장들을수행하고, 부모인경우계속하여 while() 수행
			while(1) {
				memset(buf, 0, MAXBUF);
				if (read(client_sockfd, buf, MAXBUF-1) <= 0) { // 데이터가존재할때까지계속읽음
					close(client_sockfd);
					exit(0);
				}
				printf(" > %s", buf);
				write(client_sockfd, buf, strlen(buf)); // echo
			}
		}
		if (pid == -1) { // 항상오류는검사해주어야함
			perror("fork error : ");
			return 1;
		}
	}
	close(client_sockfd);
}
