// Echo server program
#include "headerFiles.h"

#define SERV_PORT 1111

int main (void) {
	// Declare and define
	int ls, s;
	char buffer[256];
	char* ptr = buffer;
	int len = 0, n = 0, waitSize = 16, clntAddrLen;
	int maxLen = sizeof(buffer);
	struct sockaddr_in servAddr, clntAddr;

	// Create local (server) socket address
	memset (&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	// Default IP address
	servAddr.sin_port = htons(SERV_PORT);	// Default port
	
	// Create listen socket
	if(ls = socket(PF_INET, SOCK_STREAM, 0) < 0) {
		perror("Error: Listen socket failed!");
		exit(1);
	}

	// Bind listen socket to the local socket address
	if (bind (ls, &servAddr, sizeof(servAddr)) < 0) {
		perror("Error: binding failed!");
		exit(1);
	}

	// Listen to connection requests
	if (listen (ls, waitSize) < 0) {
		perror("Error: listening failed!");
		exit(1);
	}

	// Handle the connection
	for ( ; ; ) {	// Run forever
		// Accept connections from client
		if (s = accept(ls, &clntAddr, &clntAddrLen) < 0) {
			perror("Error: accepting failed!");
			exit(1);
		}

		// Data transfer section
		while ((n = recv(s, ptr, maxLen, 0)) > 0) {
			ptr += n;
			maxLen -= n;
			len += n;
		}
		send (s, buffer, len, 0);

		// Close the socket
		close(s);
	} // End of for loop
} // End of echo server program

