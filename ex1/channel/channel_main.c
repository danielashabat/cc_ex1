/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/*Authors -Daniela Shabat 316415645,<add here >.
Project – client
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

int main(int argc, char* argv[]) {
    int MyPort = CHANNEL_PORT;
    int ServerPort = SERVER_PORT;
    char IP[] = SERVER_ADDRESS_STR;
    int count = 0;
    


    SOCKET ChannelSocket = INVALID_SOCKET;
    SOCKADDR_IN ChannelAddr, ClientAddr , ServerAddr;
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

    ChannelSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP

    if (ChannelSocket == INVALID_SOCKET)
    {
        printf("Error at socket( ): %ld\n", WSAGetLastError());
        return 1;
    }

    ChannelAddr.sin_family = AF_INET;
    ChannelAddr.sin_addr.s_addr = INADDR_ANY;
    ChannelAddr.sin_port = htons(MyPort);

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR);
    ServerAddr.sin_port = htons(ServerPort); //Setting the port to connect to.
    /*bind*/

    bindRes = bind(ChannelSocket, (SOCKADDR*)&ChannelAddr, sizeof(ChannelAddr));
    if (bindRes == SOCKET_ERROR)
    {
        printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }


    //-----------------RECIEVE FROM CLIENT------------------------------
    char RecvBuf[MAX_BUFFER_SIZE];
    int AddrSize = sizeof(ClientAddr);

    /*recieving message from client*/
    count = recvfrom(ChannelSocket, RecvBuf, sizeof(RecvBuf), 0, (SOCKADDR*)&ClientAddr, &AddrSize);
    printf("-CHANNEL- recieved from client : %d bytes\n", count);
    if (count == SOCKET_ERROR) {
        printf("recvfrom failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    if (count < MAX_BUFFER_SIZE) {
        RecvBuf[count] = '\0';
    }
    else {
        printf("-SERVER ERROR- buffer is too small!\n");
        return FAIL;
    }

    //------------------------ADD NOISE-----------------------

    char* string_out = NULL;
char* hamming_reverse = NULL;
int error_count=0;
time_t t;
int len_in;

string_out = (char*)malloc(count * 8 * sizeof(char));
encoder_srting(RecvBuf, string_out, &len_in);//printd= the message in bits presentation 

char* string_with_noise = (char*)malloc(((len_in/8)+1) * sizeof(char));
int change_bits= create_noise(string_out, string_with_noise, len_in, &t, 0.01);
printf("change bits %d\n", change_bits);
printf("after noise %s\n", string_with_noise);
encoder_srting(string_with_noise, string_out, &len_in);
hamming_reverse = reverse_hamming(string_out, len_in, &error_count); // string_out

printf("after hamming reverse %s\n", hamming_reverse);


  //---------------------SEND TO SERVER--------------------------

    /*sending the message to server*/
    char SendBuf[MAX_BUFFER_SIZE];//for debuging
    strcpy(SendBuf, RecvBuf);
    int send_len = count;

    printf("-CHANNEL- sending to server : %d bytes\n", send_len);
    count = sendto(ChannelSocket, SendBuf, send_len, 0, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (count == SOCKET_ERROR) {
        printf("sendto failed with error: %d\n", WSAGetLastError());
        closesocket(ChannelSocket);
        WSACleanup();
        return FAIL;
    }
    printf("-CHANNEL- success sending to server, number of bytes sent: %d\n", count);


    /*recieving feedback from server*/
int len = sizeof(ServerAddr);
count = recvfrom(ChannelSocket, (char*)SendBuf, sizeof(SendBuf),0, (struct sockaddr*)&ServerAddr,&len);

if (count == SOCKET_ERROR) {
	printf("recvfrom failed with error %d\n", WSAGetLastError());
	return FAIL;
}
printf("-CHANNEL- recieved message from server,number of bytes recieved: %d\n", count);

/*sending the feedback to client*/
send_len = count;

printf("-CHANNEL- sending FEEDBACK to client  : %d bytes\n", send_len);
count = sendto(ChannelSocket, SendBuf, send_len, 0, (SOCKADDR*)&ClientAddr, sizeof(ClientAddr));
if (count == SOCKET_ERROR) {
    printf("sendto failed with error: %d\n", WSAGetLastError());
    closesocket(ChannelSocket);
    WSACleanup();
    return FAIL;
}
printf("-CHANNEL- success sending FEEDBACK to client, number of bytes sent: %d\n", count);



//---------------------CLOSE&CLEANUP--------------------------

    // Close the socket when finished receiving datagrams
    printf("Finished receiving. Closing socket.\n");
    iResult = closesocket(ChannelSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    //-----------------------------------------------
    // Clean up and exit.
    printf("bye channel\n");
    wprintf(L"Exiting.\n");
    WSACleanup();
    return 0;
}