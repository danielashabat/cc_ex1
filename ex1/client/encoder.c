
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>


void convert_byte_to_bits(char byte, char bits_array[]) {
	printf("given byte: 0X%hhx\n", byte);
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

void encoder(FILE* fileptr, long filelen, char* encoded_file) {

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