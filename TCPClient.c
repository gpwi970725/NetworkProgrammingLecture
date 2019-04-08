// TCP echo client program
#include "headerFiles.h"

int main(int argc, char* arg[]) {
	// Declare and define
	int s, n, servPort, len, maxLen;
	char* servName;
	char* string;
	char buffer[256+1];
	char* ptr = buffer;
	struct sockaddr_in servAddr;

	// Check and set arguments
	printf("%d %s %s %s\n", argc, arg[1], arg[2], arg[3]);
	if (argc != 4) {
		printf("Error: three arguments are needed!");
		exit(1);
	}

	servName = arg[1];
	servPort = atoi(arg[2]);
	string = arg[3];

	// Create remote (server) socket address
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	inet_pton(AF_INET, servName, &servAddr.sin_addr);
	servAddr.sin_port = htons(servPort);

	// Create socket
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Error: socket creation failed!");
		exit(1);
	}

	// Connect to the server
	if (connect (s, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("Error: connection failed!");
		exit(1);
	}

	// Data transfer section
	send (s, string, strlen(string), 0);
	while ((n = recv(s, ptr, maxLen, 0)) > 0) {
		ptr += n;
		maxLen -= n;
		len += n;
	} // End of while loop
	
	// Print and verify the echoed string
	buffer[len] = '\0';
	printf("Echoed string received: ");
	fputs(buffer, stdout);

	// Close socket
	close(s);

	// Stop program
	exit(0);
}


