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

	// TCP ����������
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}

	// ���������޾Ƶ���Ŭ���̾�Ʈ�ּҿ���Ʈ�ּҸ�����
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	// �������ּҸ����Ͽ�����
	state = bind(server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (state == -1) { // ���ϰ��ǻ���Ȯ��
		perror("bind error : ");
		exit(0);
	}

	// �����Ҽ��ִ�Ŭ���̾�Ʈ�������5���μ���
	state = listen(server_sockfd, 5);
	if (state == -1) { // ���ϰ��ǻ���Ȯ��
		perror("listen error : ");
		exit(0);
	}

	while(1) {
		// Ŭ���̾�Ʈ�ǿ������޾Ƶ���
		client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, &client_len);
		if (client_sockfd == -1) { // �׻�����°˻����־����
			perror("Accept error : ");
			exit(0);
		}
		// Ŭ���̾�Ʈ�Ϳ����̼������̸��ڽ����μ������ϳ�����
		pid = fork();
		if (pid== 0) { // �ڽ����μ����ΰ�����������������ϰ�, �θ��ΰ�����Ͽ� while() ����
			while(1) {
				memset(buf, 0, MAXBUF);
				if (read(client_sockfd, buf, MAXBUF-1) <= 0) { // �����Ͱ������Ҷ������������
					close(client_sockfd);
					exit(0);
				}
				printf(" > %s", buf);
				write(client_sockfd, buf, strlen(buf)); // echo
			}
		}
		if (pid == -1) { // �׻�����°˻����־����
			perror("fork error : ");
			return 1;
		}
	}
	close(client_sockfd);
}
