

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
#include "common.h"
#include "decoder.h"
#include "encoder.h"
#include "send_recv_tools.h"


int main(int argc, char* argv[]) {
    SOCKET ServerSocket = INVALID_SOCKET;
    SOCKADDR_IN ServerAddr, ClientAddr = { 0 };
    int bindRes;
    int iResult;
    WSADATA wsaData;
    int MyPort = SERVER_PORT;
    int MessageLen = 0;
    char RecvBuf[MAX_BUFFER_SIZE];
    int AddrSize = sizeof(ClientAddr);
    char* decoded_message;
    int errors_count=0;

    int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (StartupRes != NO_ERROR)
    {
        printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());                                 
        return FAIL;
    }
    ServerSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP
    if (ServerSocket == INVALID_SOCKET)
    {
        printf("Error at socket( ): %ld\n", WSAGetLastError());
        return 1;
    }

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(MyPort);

    bindRes = bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (bindRes == SOCKET_ERROR)
    {
        printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }

    //--------------RECIEVE FROM CHANNEL---------------------------------

    iResult = RecieveMsg(ServerSocket, RecvBuf, &MessageLen,&ClientAddr);
    if (iResult == FAIL) {
        ///cleanup
        return FAIL; }
    printf("-SERVER- recieved from client : %d bytes\n", MessageLen);

    message_decoder(RecvBuf, &decoded_message, &errors_count);
    printf("-SERVER- hamming reverse detected %d errors in message\n", errors_count);
    printf("-SERVER- message after hamming reverse: %s, size of message: %d bytes\n", decoded_message, strlen(decoded_message));
    



    //----------------------SEND FEEDBACK-------------------------
    //after recieving all messages send summary to client 
    char SendBuf[MAX_BUFFER_SIZE] = {0};//for debuging
    int send_len = 0;
    strcpy(SendBuf, "hello");
    send_len = sizeof("hello");
    printf("-SERVER- sending msg to client : %d bytes\n", MessageLen);
    MessageLen= sendto(ServerSocket, SendBuf, send_len, 0, (SOCKADDR*)&ClientAddr, AddrSize);
    printf("-SERVER- sent to client : %d bytes\n", MessageLen);



    //-----------------------------------------------


    //-----------------------------------------------
    //file handling 
  
    FILE* newfileptr;
    newfileptr = fopen("newfile.txt", "wb");  // Open the file in binary mode
    fwrite(decoded_message, 1, strlen(decoded_message), newfileptr);
    fclose(newfileptr);
    free(decoded_message);


    //------------------CLOSE&CLEANUP-----------------------------

        // Close the socket when finished receiving datagrams
    printf("Finished receiving. Closing socket.\n");
    iResult = closesocket(ServerSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    // Clean up and exit.
    printf("bye server\n");
    wprintf(L"Exiting.\n");
    WSACleanup();
	return 0;
}