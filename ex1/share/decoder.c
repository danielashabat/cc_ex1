#include <stdlib.h>
#include <string.h>
#include "decoder.h"
#include "encoder.h"

// hamming decoder
void message_decoder(char* message, int MessageLen, char ** decoded_message, int * message_decoded_len, int* errors_count) {
	int len_in;
	char* string_out = NULL;

	string_out = (char*)malloc(MessageLen * 8 * sizeof(char));
	encoder_srting(message, MessageLen, string_out, &len_in);//printd= the message in bits presentation 
	*decoded_message = reverse_hamming(string_out, len_in, errors_count, message_decoded_len); // string_out
	free(string_out);
}