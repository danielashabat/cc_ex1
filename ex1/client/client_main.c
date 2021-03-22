
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

#include "common.h"// common defines for server and client 
#include "encoder.h"


#define FILE_LEN 100//verify it




int main(int argc, char* argv[]) {

	char IP[] = SERVER_ADDRESS_STR;
	int ChannelPort = CHANNEL_PORT;
	char filename[FILE_LEN] = "myfile.txt";
	int count;
	SOCKET client_socket;
	SOCKADDR_IN RecvAddr;

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


	/*can remove connect in UDP*/

	//Create a sockaddr_in object clientService and set  values.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_addr.s_addr = inet_addr(CHANNEL_ADDRESS_STR);
	RecvAddr.sin_port = htons(ChannelPort); //Setting the port to connect to.

//---------------------------------------------
	//file handling 
	//FILE* fileptr;
	//fileptr = fopen(filename, "rb");  // Open the file in binary mode
	//long filelen;
	//char* encoded_file;//array of chars, each char value can be '0' or '1' (NOT the ASCI presentation)
	//char* hamming_send=NULL;
	//int send_len;
	//int len_in;
	//int error_count;
	//fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	//filelen = ftell(fileptr);             // Get the current byte offset in the file
	//rewind(fileptr);                      // Jump back to the beginning of the file

	//encoded_file = (char*)malloc(filelen*8*sizeof(char));//allocate memory for the encoded file 
	//encoder(fileptr, filelen, encoded_file);//encode file from bytes to bits 
	//hamming_send = hamming(filelen*8, encoded_file , &send_len);
	//char* msg_to_send_in_bits = NULL;
	//
	//msg_to_send_in_bits=(char*)malloc(send_len * 8 * sizeof(char));
	//printf("hamming_send is %s\n", hamming_send);
	////printf("number of words %d\n", send_len);
	//
	//

	//fclose(fileptr); // Close the file

	///*decoder (in server)*/
	//printf("-CLIENT-");
	//encoder_srting(hamming_send, msg_to_send_in_bits, &len_in);//debug
	//
	//char* hamming_reverse = NULL;
	//string_out[2] = 1;
	
	////printf(">>>>>>string_out[2] = %d== %c\n ", string_out[2], string_out[2]);
	//char* string_with_noise = (char*)malloc((len_in+1) * sizeof(char));
	//time_t t;
	//int change_bits= create_noise(string_out, string_with_noise, len_in, &t, 0);
	//printf("change bits %d\n", change_bits);
	//hamming_reverse = reverse_hamming(string_with_noise, len_in, &error_count); // string_out
	//
	//printf("after hamming reverse %s\n", hamming_reverse);
	//

	




//---------------------------------------------
	//sending messages

	char SendBuf[MAX_BUFFER_SIZE] ;//for debuging
	strcpy(SendBuf, "hello");
	int send_len = sizeof("hello");

	printf("-CLIENT- sending to server : %d bytes\n", send_len);
	count = sendto(client_socket, SendBuf, send_len, 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (count == SOCKET_ERROR) {
		printf("sendto failed with error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return FAIL;
	}
	printf("-CLIENT- success sending to server, number of bytes sent: %d\n", count);


	/*get feedback from channel*/
	int len = sizeof(RecvAddr);
	count = recvfrom(client_socket, (char*)SendBuf, sizeof(SendBuf), 0, (struct sockaddr*)&RecvAddr, &len);
	if (count == SOCKET_ERROR) {
		printf("recvfrom failed with error %d\n", WSAGetLastError());
		return FAIL;
	}
	if (count < MAX_BUFFER_SIZE) {
		SendBuf[count] = '\0';
	}
	else
		SendBuf[MAX_BUFFER_SIZE - 1] = '\0';
	printf("-CLIENT- recieved message from server,number of bytes recieved: %d\n", count);


	
	    // When the application is finished sending, close the socket.
    wprintf(L"Finished sending. Closing socket.\n");
    iResult = closesocket(client_socket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //---------------------------------------------
    // Clean up and quit.
	//free(encoded_file);

    wprintf(L"Exiting.\n");
    WSACleanup();
	printf("bye client\n");
	return 0;
}


