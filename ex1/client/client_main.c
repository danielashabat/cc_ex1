
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
#include <Windows.h>



#include "common.h"// common defines for server and client 
#include "encoder.h"
#include "send_recv_tools.h"




//function decleration
long  FileLen(FILE* fileptr);



int main(int argc, char* argv[]) {

	//to run with command line
	char channel_ip_str[INET_ADDRSTRLEN];
	u_short ChannelPort;
	char filename[FILE_LEN] ;

	if (argc != 4) {
		fprintf(stderr, "-ERROR- there is %d arguments, need to be 4\n", argc);
		return FAIL;
	}

	strcpy(channel_ip_str, argv[1]);
	ChannelPort = atoi(argv[2]);
	strcpy(filename, argv[3]);



	////to run without args in command line 
	//char channel_ip_str[] = CHANNEL_ADDRESS_STR;
	//int ChannelPort = CHANNEL_PORT;
	//char filename[FILE_LEN] = "test_file.txt";


	SOCKET client_socket;
	SOCKADDR_IN RecvAddr;

	// Initialize Winsock.
	WSADATA wsaData; 
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		fprintf(stderr, "-ERROR- at WSAStartup()\n");
		return FAIL;
	}
	client_socket = socket(AF_INET, SOCK_DGRAM, 0);//UDP
	if (client_socket == INVALID_SOCKET) {
		fprintf(stderr, "-ERROR- at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_addr.s_addr = inet_addr(channel_ip_str);
	RecvAddr.sin_port = htons(ChannelPort); //Setting the port to connect to.

//---------------------------------------------
	//file handling 
	FILE* fileptr = NULL;;
	long filelen;
	char* encoded_file;//array of chars, each char value can be '0' or '1' (NOT the ASCI presentation)
	char* hamming_send=NULL;
	int send_len;

	fileptr = fopen(filename, "rb");  // Open the file in binary mode
	if (fileptr == NULL) {
		fprintf(stderr, "-ERROR- fopen() failed\n");
		return FAIL;
	}
	filelen = FileLen(fileptr);
	if (filelen == 0) {
		fprintf(stderr, "-ERROR- the file is empty");
	}

	encoded_file = (char*)malloc(filelen*8*sizeof(char));//allocate memory for the encoded file 
	generate_bits_string_from_file(fileptr, filelen, encoded_file);//encode file from bytes to bits 
	hamming_send = hamming(filelen*8, encoded_file , &send_len);
	
	free(encoded_file);
	fclose(fileptr); // Close the file

//------------SEND TO CHANNEL---------------------------------
	//sending messages

	fprintf(stderr, "-CLIENT- send len: %d\n", send_len);

	int MessageLen = 0;
	char RecvBuf[MAX_BUFFER_SIZE];
	int BytesToSend = send_len;
	int RemainingBytesToSend = BytesToSend;
	int position = 0;
	int state = SEND;

	if (RemainingBytesToSend < 0) {
		fprintf(stderr, "-ERROR- there is no bytes to send\n");
		return FAIL;
	}
	while (state != EXIT) {
		switch (state)
		{
		case SEND:

			//the messages to send need to be smaller or equal to PACKET_SIZE
			if (BytesToSend > PACKET_SIZE) {
				BytesToSend = PACKET_SIZE;
			}

			printf("-CLIENT- sending to server : %d bytes\n", BytesToSend);
			iResult = SendMsg(client_socket, hamming_send + position, BytesToSend, &RecvAddr);
			if (iResult == FAIL) {
				state = EXIT;
				break;
			}
			printf("-CLIENT- success sending to server, number of bytes sent: %d\n", BytesToSend);

			RemainingBytesToSend -= BytesToSend;
			position += BytesToSend;
			BytesToSend = RemainingBytesToSend;
			if (RemainingBytesToSend > 0) {
				Sleep(100);//sleep for 1 second between sending messages
				state = SEND;
			}
			else
				state = RECIEVE;
			break;

		case RECIEVE://recieve feedback
			iResult = RecieveMsg(client_socket, RecvBuf, &MessageLen, &RecvAddr);
			if (iResult == FAIL) {
				state = EXIT;
				break;
			}
			printf("-CLIENT- recieved from server : %d bytes\n", MessageLen);
			fprintf(stderr, "%s", RecvBuf);//print feedback message
			state = EXIT;
			break;
		default:
			fprintf(stderr, "-ERROR- oops how did you get here?!\n");
			break;
		}
	}

	

    //---------------------------------------------
    // Clean up and quit.
		// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	iResult = closesocket(client_socket);
	if (iResult == SOCKET_ERROR) {
		fprintf(stderr, "-ERROR-closesocket failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	free(hamming_send);
    printf("Exiting.\n");
    WSACleanup();
	return 0;
}


//functions for clients

long  FileLen(FILE* fileptr) {
	long filelen;
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file
	return filelen;
}