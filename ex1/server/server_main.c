

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*Authors -Daniela Shabat 316415645,<add here >.
Project – server
Description –
 */
 /*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <winsock2.h>


#define SERVER_ADDRESS_STR "127.0.0.1"


int main(int argc, char* argv[]) {
    int port = 8888;
   
    SOCKET MainSocket = INVALID_SOCKET;
    unsigned long Address;
    SOCKADDR_IN service;
    int bindRes;
    int ListenRes;

    // Initialize Winsock.
    WSADATA wsaData;
    int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET AcceptSocket;
    int program_running = 1;
    int i = 0;
    if (StartupRes != NO_ERROR)
    {
        printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
        // Tell the user that we could not find a usable WinSock DLL.                                  
        return;
    }

    MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (MainSocket == INVALID_SOCKET)
    {
        printf("Error at socket( ): %ld\n", WSAGetLastError());
        return 1;
    }

    //create a sockaddr_in 

    Address = inet_addr(SERVER_ADDRESS_STR);
    if (Address == INADDR_NONE)
    {
        printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
            SERVER_ADDRESS_STR);
        return 1;;
    }

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = Address;
    service.sin_port = htons(port);
    /*bind*/

    bindRes = bind(MainSocket, (SOCKADDR*)&service, sizeof(service));
    if (bindRes == SOCKET_ERROR)
    {
        printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }


	printf("bye server\n");
	return 0;
}