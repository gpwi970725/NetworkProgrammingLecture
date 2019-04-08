//UDP echo server program
#include "headerFiles.h"
int main(void) {
	//Declare and define variables
	int s;							// Socket descriptor (reference)
	int len;						// Length of string to be echoed
	char buffer[256];				// Data buffer
	struct sockaddr_in servAddr;	// Server (local) socket addrss
	struct sockaddr_in cIntAddr;	// Client (remote) socket address
	int cIntAddrLen;				// Length of client socket address

	// Build local (server) socket address
	memset(&servAddr, 0, sizeof(servAddr));			// Allocate memory
	servAddr.sin_family = AF_INET;					// Family field
	servAddr.sin_port = htons(SERVER_PORT);			// Default port number
	servAddr.sin_addr.s_addr = htonl(INADR_ANY);	// Default IP address
	// Create socket
	if (s = sockt (PF_INET, SOCK_DGRAM, 0) < 0) {
		perror("Error:socket failed!");
		exit(1);
	}

	// Bind socket to local address and port
	if (bind (s, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("Error: bind failed!");
		exit(1);
	}

	for( ; ; ) {		// Run forever
		// Receive String
		len = recvfrom((s, buffer, sizeof(buffer), 0,
					   struct sockaddr*)&cIntAddr, &cIntAddrLen);
		// Send String
		sendto(s, buffer, len, 0, (struct sockaddr*)&cIntAddr, sizeof(cIntAddr));
	}// End of for loop
}// End of echo server program