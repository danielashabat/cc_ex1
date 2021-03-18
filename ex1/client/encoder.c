
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

void encoder_srting(char* string_in, char* string_out) {
	unsigned char bits[8] = { 0 };
	
	for (int i = 0; i < strlen(string_in); i++) {
		convert_byte_to_bits(string_in[i], bits);
		memcpy(string_out + i * 8, bits, 8);
	}
	printf(">>>>>>encoded string: ");
	for (int i = 0; i < strlen(string_in) * 8; i++) {
		printf("%d", string_out[i]);
	}
	printf("\n");
}



char* hamming(int input_len, char* encoded_file, int *send_len) {
	
	int frames_num = input_len / 11;
	int module_frames= input_len % 11;
	char *output = (char*)calloc((frames_num * 15)+1, sizeof(char));
	
	int c_1, c_2, c_4, c_8;
	char d3, d5, d6, d7, d9, d10, d11, d12,d13,d14 ,d15;
	printf(">>>>%d frames, %d left\n", frames_num, module_frames);
	for (int i = 0; i < frames_num; i++) { // frames_num
		c_1=0, c_2=0, c_4=0, c_8=0;
		if (encoded_file[(1 - 1) + (11 * i)] == 1) {
			d3 = '1';
			c_1++;
		}
		else d3 = '0';

		if (encoded_file[(2 - 1) + (11 * i)] == 1) {
			d5 = '1';
			c_1++;
			c_4++;
		}
		else d5 = '0';

		if (encoded_file[(3 - 1) + (11 * i)] == 1) {
			d6 = '1';
			c_2++;
			c_4++;
		}
		else d6 = '0';

		if (encoded_file[(4 - 1) + (11 * i)] == 1) {
			d7 = '1';
			c_1++;
			c_2++;
			c_4++;
		}
		else d7 = '0';

		if (encoded_file[(5 - 1) + (11 * i)] == 1) {
			d9 = '1';
			c_1++;
			c_8++;
		}
		else d9 = '0';

		if (encoded_file[(6 - 1) + (11 * i)] == 1) {
			d10 = '1';
			c_2++;
			c_8++;
		}
		else d10 = '0';

		if (encoded_file[(7 - 1) + (11 * i)] == 1) {
			d11 = '1';
			c_1++;
			c_2++;
			c_8++;
		}
		else d11 = '0';

		if (encoded_file[(8 - 1) + (11 * i)] == 1) {
			d12 = '1';
			c_4++;
			c_8++;
		}
		else d12 = '0';

		if (encoded_file[(9 - 1) + (11 * i)] == 1) {
			d13 = '1';
			c_1++;
			c_4++;
			c_8++;
		}
		else d13 = '0';

		if (encoded_file[(10 - 1) + (11 * i)] == 1) {
			d14 = '1';
			c_2++;
			c_4++;
			c_8++;
		}
		else d14 = '0';

		if (encoded_file[(11 - 1) + (11 * i)] == 1) {
			d15 = '1';
			c_1++;
			c_2++;
			c_4++;
			c_8++;
		}
		else d15 = '0';

		
		output[3 - 1 + (15 * i)] = d3;
		output[5 - 1 + (15 * i)] = d5;
		output[6 - 1 + (15 * i)] = d6;
		output[7 - 1 + (15 * i)] = d7;
		output[9 - 1 + (15 * i)] = d9;
		output[10 - 1 + (15 * i)] = d10;
		output[11 - 1 + (15 * i)] = d11;
		output[12 - 1 + (15 * i)] = d12;
		output[13 - 1 + (15 * i)] = d13;
		output[14 - 1 + (15 * i)] = d14;
		output[15 - 1 + (15 * i)] = d15;
		output[1 - 1 + (15 * i)] = (c_1 % 2) ? '1' : '0';
		output[2 - 1 + (15 * i)] = (c_2 % 2) ? '1' : '0';
		output[4 - 1 + (15 * i)] = (c_4 % 2) ? '1' : '0';
		output[8 - 1 + (15 * i)] = (c_8 % 2) ? '1' : '0';

	
	}
	output[(frames_num * 15)] = '\0';
	printf(">>>output is %s\n", output);

	int number_of_letters = ((frames_num * 15) % 8 == 0) ? ((frames_num * 15) / 8) : (((frames_num * 15) / 8) + 1);
	char* output_letters = (char*)calloc(number_of_letters + 1, sizeof(char));
	*send_len = number_of_letters;
	char temp[9];
	//char c;
	for (int j = 0; j < number_of_letters; j++) { ///number_of_letters
		temp[0] = output[(1 - 1) + (8 * j)];
		temp[1] = output[(2 - 1) + (8 * j)];
		temp[2] = output[(3 - 1) + (8 * j)];
		temp[3] = output[(4 - 1) + (8 * j)];
		temp[4] = output[(5 - 1) + (8 * j)];
		temp[5] = output[(6 - 1) + (8 * j)];
		temp[6] = output[(7 - 1) + (8 * j)];
		temp[7] = output[(8 - 1) + (8 * j)];
		temp[8] = '\0';
		output_letters[j]= strtol(temp, 0, 2);
	}
	output_letters[number_of_letters] = '\0';
	printf("the output in letters %s\n", output_letters);
	return output_letters;
	//output[frames_num * 15] = '\0';
	//convert_int_to_string(output_int, frames_num * 15, output);
	//printf(">>>>%s output\n", output);
	//char* data = "01010110";
	/*char* data = "11001";
	char c = strtol(data, 0, 2);
	printf("%s = %c = %d = 0x%.2X\n", data, c, c, c);*/
}

void reverse_hamming(char* in, char* out) {

}