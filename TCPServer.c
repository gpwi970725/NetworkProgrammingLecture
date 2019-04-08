//소켓프로그래밍에사용될헤더파일선언
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"

//메시지송수신에사용될버퍼크기를선언
#define MAXBUF 1024

// This is TCP-based server Program
// Author :
//date:

int main(int argc,char *argv[]) {
	char buffer[MAXBUF];
	struct sockaddr_in server_addr, client_addr;
	char temp[20];
	//serverfd,clientfd:소켓번호
	int serverfd, clientfd;
	int len, msg_size;
	if(argc!=2) {
		printf("usage : %s [port] \n", argv[0]);
		exit(0);
	}

	//소켓생성
	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // 오류가발생하면
		printf("Server : Can't create socket \n");
		exit(0);
	}

	//server_Addr을NULL로초기화
	memset(&server_addr, 0, sizeof(server_addr));

	//server_addr셋팅
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	//bind()호출, 생성된소켓에입력한서버주소를연결
	if(bind(serverfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0) {
		printf("Server : Can't bind local address of server \n");
		exit(0);
	}

	//소켓을수동대기모드로설정,
	//5는서버에서연결가능한클라이언트수를의미함
	if(listen(serverfd,5)<0) {
		printf("Server:Can'tlisten\n");
		exit(0);
	}
	memset(buffer, 0, sizeof(buffer));

	len = sizeof(client_addr);

	// 아래의프로그램은클라이언트로부터연결한후클라이언트와서버가서로데이터를한번만송수신하고
	// 소켓연결을끊고, 다시연결을기다리는과정을계속수행한다.
	while(1) {
		// 클라이언트의요청을감지하면accept 함수가실행되고그반환값으로클라이언트의소켓번호가생성됩니다.
		clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &len);
		if(clientfd<0) {
			printf("Server : failed to accept the client requests.\n");
			exit(0);
		}
		// 클라이언트주소를얻음
		inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
		printf("Server : %s client connected. \n", temp);

		msg_size = read(clientfd, buffer, MAXBUF);
		write(clientfd,buffer,msg_size);
		// 클라이언트와연결을끊음
		close(clientfd);
		printf("Server : %s client closed. \n", temp);
	}
	close(serverfd);
	return 0;
}
