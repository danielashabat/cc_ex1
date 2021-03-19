#ifndef	ENCODER_H
#define	ENCODER_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

void convert_byte_to_bits(char byte, char bits_array[]);
void encoder(FILE* fileptr, long filelen, char* encoded_file);
char* hamming(int input_len, char* encoded_file,  int* send_len);
void encoder_srting(char* string_in, char* string_out, int* len);
char* reverse_hamming(char* in, int len_in);
#endif