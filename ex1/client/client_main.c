
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*Authors -Daniela Shabat 316415645,<add here >.
Project – client
Description –  
 */
 /*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/


#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>

#define SERVER_ADDRESS_STR "127.0.0.1"


//debug 
#define DEBUG(MSG) printf("function:[%s line:%d] %s\n", __func__, __LINE__,MSG)

typedef enum { FAIL=1} return_values;

//

int main(int argc, char* argv[]) {

	char IP[] = SERVER_ADDRESS_STR;
	int port = 8888;

	SOCKET client_socket;
	SOCKADDR_IN clientService;

	// Initialize Winsock.
	WSADATA wsaData; //Create a WSADATA object called wsaData.
					 //The WSADATA structure contains information about the Windows Sockets implementation.

					 //Call WSAStartup and check for errors.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		DEBUG("Error at WSAStartup()\n");
		return FAIL;
	}
	// Create a socket.
	client_socket = socket(AF_INET, SOCK_DGRAM, 0);//UDP

	// Check for errors to ensure that the socket is a valid socket.
	if (client_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}


	//can remove connect in UDP

	////Create a sockaddr_in object clientService and set  values.
	//clientService.sin_family = AF_INET;
	//InetPton(AF_INET, SERVER_ADDRESS_STR, &clientService.sin_addr.s_addr);
	//clientService.sin_port = htons(port); //Setting the port to connect to.

	//									  /*
	//									  AF_INET is the Internet address family.
	//									  */
	//									  // Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	//									  // Check for general errors.

	//if (SOCKET_ERROR == connect(client_socket, (SOCKADDR*)&clientService, sizeof(clientService))) {
	//	closesocket(client_socket);
	//	WSACleanup();
	//	return 1;
	//}

	//printf("Connected to server on %s:%d\n", IP, port);

	printf("bye client\n");
	return 0;
}