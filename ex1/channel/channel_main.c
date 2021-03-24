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
#include "send_recv_tools.h"
#include "encoder.h"

int main(int argc, char* argv[]) {
    int MyPort = CHANNEL_PORT;
    int ServerPort = SERVER_PORT;
    char IP[] = SERVER_ADDRESS_STR;
    int count = 0;
  
    SOCKET ChannelSocket = INVALID_SOCKET;
    SOCKADDR_IN ChannelAddr, ClientAddr = {0}, ServerAddr, RecvAddr, SendAddr;
    int bindRes;
    int iResult;
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
    char SendBuf[MAX_BUFFER_SIZE];
    int AddrSize = sizeof(ClientAddr);
    int MessageLen = 0;
    int BytesToSend =0;
    int state = RECIEVE;
    int feedback_message = 0;
    char* string_out = NULL;
    char* hamming_reverse = NULL;
    int error_count = 0;
    time_t t;
    int len_in = 0;

    RecvAddr = ServerAddr;
    while (state != EXIT) {
        switch (state)
        {
        case SEND:
            printf("-CHANNEL- sending to reciever : %d bytes\n", BytesToSend);
            iResult = SendMsg(ChannelSocket, SendBuf, BytesToSend, &RecvAddr);
            if (iResult == FAIL) {
                state = EXIT;
                break;
            }
            printf("-CHANNEL- success sending to reciever, number of bytes sent: %d\n", BytesToSend);
            if (feedback_message) {
                state = EXIT;
            }
            else {
                state = RECIEVE;
            }
            ClientAddr = SendAddr;//save the client address
            break;

        case RECIEVE:
            iResult = RecieveMsg(ChannelSocket, RecvBuf,&MessageLen, &SendAddr);
            if (iResult == FAIL) {
                state = EXIT;
                break;
            }
            printf("-CHANNEL- recieved from sender : %d bytes\n", MessageLen);
            BytesToSend = MessageLen;
            if (SendAddr.sin_addr.s_addr == ServerAddr.sin_addr.s_addr && ServerAddr.sin_port == SendAddr.sin_port) {//check if it's feedback message from server
                RecvAddr = ClientAddr;
                printf("-CHANNEL- recognized FEEDBACK message\n");
                strcpy(SendBuf ,RecvBuf);
                feedback_message = 1;
                state = SEND;
            }
            else {
                state = ADD_NOISE;
            }
            break;

        case ADD_NOISE:
            string_out = (char*)malloc(MessageLen * 8 * sizeof(char));
            encoder_srting(RecvBuf, string_out, &len_in);//printd= the message in bits presentation 

            char* string_with_noise = (char*)malloc(((len_in / 8) + 1) * sizeof(char));
            int change_bits = create_noise(string_out, SendBuf, len_in, &t, 0.01);
            printf("change bits %d\n", change_bits);
            printf("after noise %s\n", string_with_noise);
            state = SEND;
            break;

        default:
            printf("-ERROR- oops how did you get here?!\n");
            break;
        }
    }

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