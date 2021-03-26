

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
#include <conio.h>
#include "common.h"
#include "decoder.h"
#include "encoder.h"
#include "send_recv_tools.h"


int user_typed_End();
int new_message_arrived(SOCKET socket, fd_set set);
void create_feedback_message(char* feedback_message, int total_recieved_bytes, int total_wrote_bytes, int total_errors);

int main(int argc, char* argv[]) {
    ///to run with command line
    u_short MyPort;
    char NewFileName[FILE_LEN];

    if (argc != 3) {
        fprintf(stderr, "-ERROR- there is %d arguments, need to be 3\n", argc);
        return FAIL;
    }
    MyPort = atoi(argv[1]);
    strcpy(NewFileName, argv[2]);


    ////to run without arguments in command line
    //u_short MyPort = SERVER_PORT;
    //char NewFileName[FILE_LEN]="new_text.txt";


    SOCKET ServerSocket = INVALID_SOCKET;
    SOCKADDR_IN ServerAddr, ClientAddr = { 0 };
    int bindRes;
    int iResult;
    WSADATA wsaData;
    int MessageLen = 0;
    char RecvBuf[MAX_BUFFER_SIZE];
    int AddrSize = sizeof(ClientAddr);
    char* decoded_message=NULL;
    int message_decoded_len = 0;
    int errors_count = 0;
    char feedback_message[FEEDBACK_MSG_LEN] = { 0 };

    int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (StartupRes != NO_ERROR)
    {
        fprintf(stderr, "-ERROR- %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
        return FAIL;
    }
    ServerSocket = socket(AF_INET, SOCK_DGRAM, 0);//UDP
    if (ServerSocket == INVALID_SOCKET)
    {
        fprintf(stderr, "-ERROR- socket( ) failed: %ld\n", WSAGetLastError());
        return 1;
    }
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(MyPort);

    bindRes = bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
    if (bindRes == SOCKET_ERROR)
    {
        fprintf(stderr, "-ERROR- bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
        return 1;
    }
    
    fd_set set ;
    FD_ZERO(&set); /* clear the set */
    int BytesToSend = 0;
    int state = RECIEVE;
    int total_recieved_bytes=0, total_wrote_bytes=0, total_errors=0;
    FILE* newfileptr = NULL;

    newfileptr = fopen(NewFileName, "wb");  // Open the file in binary mode
    if (newfileptr == NULL) {
        fprintf(stderr, "-ERROR- fopen() failed");
        return FAIL;
    }

    printf("Type 'End' when done\n");
    while (state != EXIT) {
        switch (state)
        {
        case SEND:
            create_feedback_message(feedback_message, total_recieved_bytes, total_wrote_bytes, total_errors);
            fprintf(stderr,"%s", feedback_message);
            BytesToSend = strlen(feedback_message);
            printf("-SERVER- sending to server : %d bytes\n", BytesToSend);
            iResult = SendMsg(ServerSocket, feedback_message, BytesToSend, &ClientAddr);
            if (iResult == FAIL) {
                state = EXIT;
                break;
            }
            printf("-SERVER- success sending to server, number of bytes sent: %d\n", BytesToSend);

            state = EXIT;
            break;

        case RECIEVE:
            if (user_typed_End()) { state = SEND; break; } //when 'End' is typed, send feedback

            if (new_message_arrived(ServerSocket, set))
            {
                iResult = RecieveMsg(ServerSocket, RecvBuf, &MessageLen, &ClientAddr);
                if (iResult == FAIL) {
                    state = EXIT;
                    break;
                }
                printf("-SERVER- recieved from client : %d bytes\n", MessageLen);
                state = DECODE;
            }
            break;

        case DECODE:
            message_decoder(RecvBuf, MessageLen, &decoded_message, &message_decoded_len, &errors_count);
            total_errors += errors_count;
            total_recieved_bytes += MessageLen;
            total_wrote_bytes += message_decoded_len;
            printf("-SERVER- bytes wrote: %d\n", message_decoded_len);
            fwrite(decoded_message, 1, message_decoded_len, newfileptr);
            free(decoded_message);
            state = RECIEVE;
            break;
        default:
            fprintf(stderr, "-ERROR- oops how did you get here?!\n");
            break;
        }
    }


    //------------------CLOSE&CLEANUP-----------------------------

    fclose(newfileptr);
    printf("Finished receiving. Closing socket.\n");
    iResult = closesocket(ServerSocket);
    if (iResult == SOCKET_ERROR) {
        fprintf(stderr, "-ERROR- closesocket failed with error %d\n", WSAGetLastError());
        return FAIL;
    }
    // Clean up and exit.
    printf("bye server\n");
    WSACleanup();
    return 0;
    }

    //functions for sevrer

    //return 1 if user type 'End' in stdin, else return 0.
    int user_typed_End() {
        
        int i = 0;
        char input[4] = { 0 };
        char exit_arr[3] = { 'E','n','d' };//need to chande!!!!!

        if (_kbhit()) {
            
            for (int i = 0; i < 3; i++) {
                input[i]= _getch();
                if (exit_arr[i] != input[i]){
                    return 0;
                }
             }
            return 1;
        }
       
        return 0;
}

    int new_message_arrived(SOCKET socket, fd_set set) {
        struct timeval time_out;
        time_out.tv_sec = 0;//poll
        time_out.tv_usec = 0;

        FD_ZERO(&set); /* clear the set */
        FD_SET(socket, &set);
        int rv = select(socket + 1, &set, NULL, NULL, &time_out);
        if (rv == SOCKET_ERROR) {
            fprintf(stderr, "-ERROR- select failed. socket error\n");
            return 0;
        }
        if (rv== 0)
            return 0;
        else
            return 1;
    }

    void create_feedback_message(char* feedback_message,int total_recieved_bytes, int total_wrote_bytes, int total_errors) {
        
        sprintf(feedback_message,"recieved: %d bytes\nwrote: %d bytes\ndetected & corrected %d errors\n", total_recieved_bytes, total_wrote_bytes, total_errors);
    }