#ifndef	ENCODER_H
#define	ENCODER_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

void convert_byte_to_bits(char byte, char bits_array[]);
void encoder(FILE* fileptr, long filelen, char* encoded_file);

#endif