

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*Authors -Daniela Shabat 316415645,<add here >.
Project – server
Description –
 */
 /*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "encoder.h"
#include "common.h"


int main(int argc, char* argv[]) {
    int port = 8888;
    char IP[] =SERVER_ADDRESS_STR;
    int count = 0;
   
    SOCKET RecvSocket = INVALID_SOCKET;
    SOCKADDR_IN ServerAddr;
    int bindRes;
    int iResult;

    

    // Initialize Winsock.
    WSADATA wsaData;
    int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    int program_running = 1;
    int i = 0;
    if (StartupRes != NO_ERROR)
    {
        printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
        // Tell the user that we could not find a usable WinSock DLL.                                  
        return;
    }

    RecvSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP

    if (RecvSocket == INVALID_SOCKET)
    {
        printf("Error at socket( ): %ld\n", WSAGetLastError());
        return 1;
    }

    //create a sockaddr_in 
    

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR);
    ServerAddr.sin_port = htons(port);
    /*bind*/

    bindRes = bind(RecvSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (bindRes == SOCKET_ERROR)
    {
        printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }

    char RecvBuf[MAX_BUFFER_SIZE];
    int ServerAddrSize = sizeof(ServerAddr);


   
    count = recvfrom(RecvSocket,
        RecvBuf, MAX_BUFFER_SIZE, 0, (SOCKADDR*)&ServerAddr, &ServerAddrSize);
    if (count == SOCKET_ERROR) {
        wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    printf("-SERVER- recieved from client : %s\n", RecvBuf);
    // Close the socket when finished receiving datagrams
    wprintf(L"Finished receiving. Closing socket.\n");
    iResult = closesocket(RecvSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    //-----------------------------------------------

    //file handling 
  
    //FILE* newfileptr;
    //newfileptr = fopen("newfile.jpg", "wb");  // Open the file in binary mode
    //fwrite(temp, 1, filelen, newfileptr);
    //fclose(newfileptr);



    //-----------------------------------------------
    // Clean up and exit.
    printf("bye server\n");
    wprintf(L"Exiting.\n");
    WSACleanup();
	return 0;
}