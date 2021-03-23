#include <stdlib.h>
#include <string.h>
#include "decoder.h"

// hamming decoder
void message_decoder(char* message, char ** decoded_message, int* errors_count) {
	int len_in;
	char* string_out = NULL;

	string_out = (char*)malloc(strlen(message) * 8 * sizeof(char));
	encoder_srting(message, string_out, &len_in);//printd= the message in bits presentation 
	*decoded_message = reverse_hamming(string_out, len_in, errors_count); // string_out
	free(string_out);
}