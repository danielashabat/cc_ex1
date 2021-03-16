
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
#define INFORMATION_LEN_BITS 16
#define BUF_SIZE 2//buffer size in bytes

void convert_byte_to_bits(char byte, char bits_array[]);
void encoder(FILE* fileptr, long filelen, char* encoded_file);

int main(int argc, char* argv[]) {

	char IP[] = SERVER_ADDRESS_STR;
	int port = 8888;
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
	RecvAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR);
	RecvAddr.sin_port = htons(port); //Setting the port to connect to.

//---------------------------------------------
	//file handling 
	FILE* fileptr;
	fileptr = fopen("myfile.txt", "rb");  // Open the file in binary mode
	long filelen;
	char* encoded_file;

	
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	encoded_file = (char*)malloc(filelen*8);//allocate memory for the bits file 
	encoder(fileptr, filelen, encoded_file);//encode file from bytes to bits 

	
	fclose(fileptr); // Close the file

	//insert hamming code 






//---------------------------------------------
	char SendBuf[100] ="hello";//for debuging

	printf("-CLIENT- sending to server : %s\n", SendBuf);
	count = sendto(client_socket,SendBuf, sizeof(SendBuf), 0, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr));
	if (count == SOCKET_ERROR) {
		wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return FAIL;
	}


	free(encoded_file);
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
    wprintf(L"Exiting.\n");
    WSACleanup();
	printf("bye client\n");
	return 0;
}


void convert_byte_to_bits(char byte, char bits_array[]) {
	printf("given byte: 0X%hhx\n",byte);
	unsigned char mask = 1; // Bit mask

	// Extract the bits
	for (int i = 0; i < 8; i++) {
		// Mask each bit in the byte and store it
		bits_array[8 - i - 1] = (byte & (mask << i)) != 0;
	}
	// For debug purposes, lets print the received data
	printf("convert: ");
	for (int i = 0; i < 8; i++) {
		printf("%d", bits_array[i]);
	}
	printf("\n");
	return;
}

void encoder(FILE* fileptr,long filelen, char* encoded_file) {

	char buffer;
	unsigned char bits[8] = { 0 };

	for (int i = 0; i < filelen; i++) {
		fread(&buffer, 1, 1, fileptr); // Read in the entire file
		convert_byte_to_bits(buffer, bits);
		memcpy(encoded_file + i * 8, bits, 8);
	}

	printf("encoded file: ");
	for (int i = 0; i < filelen * 8; i++) {
		printf("%d", encoded_file[i]);
	}
	printf("\n");

}