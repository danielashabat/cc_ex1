

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*Authors -Daniela Shabat 316415645,<add here >.
Project – server
Description –
 */
 /*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

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
   
    SOCKET ServerSocket = INVALID_SOCKET;
    SOCKADDR_IN ServerAddr, ClientAddr;
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

    ServerSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP

    if (ServerSocket == INVALID_SOCKET)
    {
        printf("Error at socket( ): %ld\n", WSAGetLastError());
        return 1;
    }

    //create a sockaddr_in 
    

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR);
    ServerAddr.sin_port = htons(port);
    /*bind*/

    bindRes = bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (bindRes == SOCKET_ERROR)
    {
        printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }

    char RecvBuf[MAX_BUFFER_SIZE];
    int AddrSize = sizeof(ClientAddr);

   
    count = recvfrom(ServerSocket, RecvBuf, sizeof(RecvBuf), 0, (SOCKADDR*)&ClientAddr, &AddrSize);
    printf("-SERVER- recieved from client : %d bytes\n", count);
    if (count < MAX_BUFFER_SIZE) {
        RecvBuf[count] = '\0';
    }
    if (count == SOCKET_ERROR) {
        printf(L"recvfrom failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    


    ////after recieving all messages send summary to client 
    //char SendBuf[MAX_BUFFER_SIZE] = "Server ends";//for debuging
    //sendto(ServerSocket, SendBuf, MAX_BUFFER_SIZE, 0, (const struct sockaddr*)&ClientAddr, AddrSize);
    //printf("-SERVER- message sent to client .\n");


    // Close the socket when finished receiving datagrams
    printf("Finished receiving. Closing socket.\n");
    iResult = closesocket(ServerSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    //-----------------------------------------------

     // hamming decoder
    int send_len = strlen(RecvBuf);
    int len_in;
    int error_count;
    char* hamming_reverse = NULL;
    char* string_out = NULL;

    printf("-SERVER- send len is: %d\n", send_len);
   
    string_out = (char*)malloc(send_len * 8 * sizeof(char));
    encoder_srting(RecvBuf, string_out, &len_in);//printd= the message in bits presentation 

   
    hamming_reverse = reverse_hamming(string_out, len_in, &error_count); // string_out
    printf("after hamming reverse: %s\n", hamming_reverse);
    //-----------------------------------------------
    //file handling 
  
    FILE* newfileptr;
    newfileptr = fopen("newfile.txt", "wb");  // Open the file in binary mode
    fwrite(hamming_reverse, 1, sizeof(hamming_reverse), newfileptr);
    fclose(newfileptr);



    //-----------------------------------------------
    // Clean up and exit.
    printf("bye server\n");
    wprintf(L"Exiting.\n");
    WSACleanup();
	return 0;
}