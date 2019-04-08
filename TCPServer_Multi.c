//�������α׷��ֿ�����������ϼ���
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

//�޽����ۼ��ſ����ɹ���ũ�⸦����
#define MAXBUF 1024

// This is TCP-based server Program
// Author :
//date:

int main(int argc,char *argv[]) {
	char buffer[MAXBUF];
	struct sockaddr_in server_addr, client_addr;
	char temp[20];
	//serverfd,clientfd:���Ϲ�ȣ
	int serverfd, clientfd;
	int len, msg_size;
	if(argc!=2) {
		printf("usage : %s [port] \n", argv[0]);
		exit(0);
	}

	//���ϻ���
	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // �������߻��ϸ�
		printf("Server : Can't create socket \n");
		exit(0);
	}

	//server_Addr��NULL���ʱ�ȭ
	memset(&server_addr, 0, sizeof(server_addr));

	//server_addr����
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	//bind()ȣ��, �����ȼ��Ͽ��Է��Ѽ����ּҸ�����
	if(bind(serverfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0) {
		printf("Server : Can't bind local address of server \n");
		exit(0);
	}

	//���������������μ���,
	//5�¼����������ᰡ����Ŭ���̾�Ʈ�����ǹ���
	if(listen(serverfd,5)<0) {
		printf("Server:Can'tlisten\n");
		exit(0);
	}
	memset(buffer, 0, sizeof(buffer));

	len = sizeof(client_addr);

	// �Ʒ������α׷���Ŭ���̾�Ʈ�κ��Ϳ�������Ŭ���̾�Ʈ�ͼ��������ε����͸��ѹ����ۼ����ϰ�
	// ���Ͽ���������, �ٽÿ�������ٸ��°�������Ӽ����Ѵ�.
	while(1) {
		// Ŭ���̾�Ʈ�ǿ�û�������ϸ�accept �Լ�������ǰ�׹�ȯ������Ŭ���̾�Ʈ�Ǽ��Ϲ�ȣ�������˴ϴ�.
		clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd<0) {
			printf("Server : failed to accept the client requests.\n");
			exit(0);
		}
		// Ŭ���̾�Ʈ�ּҸ�����
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("Server : %s client connected. \n", temp);

		msg_size = read(clientfd, buffer, MAXBUF);
		write(clientfd,buffer,msg_size);
		// Ŭ���̾�Ʈ�Ϳ���������
		close(clientfd);
		printf("Server : %s client closed. \n", temp);
	}
	close(serverfd);
	return 0;
}
