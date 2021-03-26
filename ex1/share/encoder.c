
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>


void convert_byte_to_bits(char byte, char bits_array[]) {
	//printf("given byte: 0X%hhx\n", byte);
	unsigned char mask = 1; // Bit mask

	// Extract the bits
	for (int i = 0; i < 8; i++) {
		// Mask each bit in the byte and store it
		bits_array[8 - i - 1] = (byte & (mask << i)) != 0;
	}
	// For debug purposes, lets print the received data
	//printf("convert: ");
	//for (int i = 0; i < 8; i++) {
	//	printf("%d", bits_array[i]);
	//}
	//printf("\n");
	return;
}


//read file in bytes and convert the file to string of 0/1
void generate_bits_string_from_file(FILE* fileptr, long filelen, char* encoded_file) {

	char buffer;
	unsigned char bits[8] = { 0 };

	for (int i = 0; i < filelen; i++) {
		fread(&buffer, 1, 1, fileptr); // Read in the entire file
		convert_byte_to_bits(buffer, bits);
		memcpy(encoded_file + i * 8, bits, 8);
	}

	//printf("encoded file: \n");
	//for (int i = 0; i < filelen * 8; i++) {
	//	printf("%d", encoded_file[i]);
	//}
	//printf("\n");

}

void encoder_srting(char* string_in, char* string_out, int* len) { // reciving a string of chars, returning a string of 0/1 
	unsigned char bits[8] = { 0 };
	
	for (int i = 0; i < strlen(string_in); i++) {
		convert_byte_to_bits(string_in[i], bits);
		memcpy(string_out + i * 8, bits, 8);
	}
	*len = strlen(string_in) * 8;
	//printf("encoded string: ");
	//for (int i = 0; i < strlen(string_in) * 8; i++) {
	//	printf("%d", string_out[i]);
	//}
	//printf("\n");
}



char* hamming(int input_len, char* encoded_file, int *send_len) { ///reciving string of chars 0/1 and its len. return string of alphabet after hamming, and its length.
	
	int frames_num = input_len / 11;
	int module_frames= input_len % 11;
	char *output = (char*)calloc((frames_num * 15)+1, sizeof(char));
	
	int c_1, c_2, c_4, c_8;
	char d3, d5, d6, d7, d9, d10, d11, d12,d13,d14 ,d15;
	//printf(">>>>%d frames, %d left\n", frames_num, module_frames);
	for (int i = 0; i < frames_num; i++) { // frames_num
		c_1=0, c_2=0, c_4=0, c_8=0;
		if (encoded_file[(1 - 1) + (11 * i)] == 1) {
			d3 = '1';
			c_1++;
			c_2++; //new
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
	//printf(">>>output is %s\n", output);

	int number_of_letters = ((frames_num * 15) % 8 == 0) ? ((frames_num * 15) / 8) : (((frames_num * 15) / 8) + 1);
	char* output_letters = (char*)calloc(number_of_letters + 1, sizeof(char));
	*send_len = number_of_letters;
	char temp[9];
	//char c;
	for (int j = 0; j < number_of_letters - 1; j++) { ///number_of_letters
		temp[0] = output[(1 - 1) + (8 * j)];
		temp[1] = output[(2 - 1) + (8 * j)];
		temp[2] = output[(3 - 1) + (8 * j)];
		temp[3] = output[(4 - 1) + (8 * j)];
		temp[4] = output[(5 - 1) + (8 * j)];
		temp[5] = output[(6 - 1) + (8 * j)];
		temp[6] = output[(7 - 1) + (8 * j)];
		temp[7] = output[(8 - 1) + (8 * j)];
		temp[8] = '\0';
		output_letters[j] = strtol(temp, 0, 2);
	}
	if ((frames_num * 15) % 8 == 0){
		for (int j = number_of_letters - 1; j < number_of_letters ; j++) { ///number_of_letters
			temp[0] = output[(1 - 1) + (8 * j)];
			temp[1] = output[(2 - 1) + (8 * j)];
			temp[2] = output[(3 - 1) + (8 * j)];
			temp[3] = output[(4 - 1) + (8 * j)];
			temp[4] = output[(5 - 1) + (8 * j)];
			temp[5] = output[(6 - 1) + (8 * j)];
			temp[6] = output[(7 - 1) + (8 * j)];
			temp[7] = output[(8 - 1) + (8 * j)];
			temp[8] = '\0';
			output_letters[j] = strtol(temp, 0, 2);
		}

}
	else {
		temp[0] = temp[1] = temp[2] = temp[3] = temp[4] = temp[5] = temp[6] = temp[7] = '0';
		for (int j = 0; j < ((frames_num * 15) % 8); j++) {
			temp[j] = output[j + (8 * (number_of_letters - 1))];

		}
		temp[8] = '\0';
		output_letters[number_of_letters - 1] = strtol(temp, 0, 2);
	}
	output_letters[number_of_letters] = '\0';
	free(output);
	//printf("the output in letters %s\n", output_letters);
	return output_letters;
	
}

char* reverse_hamming(char* in, int len, int *errors) { ///reciving string of chars==0/1 (not '0' or '1') and its len. returning string of alphabet after reverse hamming.
	//printf("the len of the input is %d\n", len);
	int efective_len = len - len % 15;
	//printf("the efective len of the input is %d\n", efective_len);
	int frames_num = efective_len / 15;
	int checks_sum[4];
	int check_index = 0;
	*errors = 0;
	//int errors=0;
	//char* input_fixed= (char*)calloc((frames_num * 15) + 1, sizeof(char));
	char* output = (char*)calloc((frames_num * 11) + 1, sizeof(char));
	char data[16] = {'0'};
	int c_1=0, c_2=0, c_4=0, c_8=0;
	for (int i = 0; i < frames_num; i++) {
		c_1 = 0, c_2 = 0, c_4 = 0, c_8 = 0;
		if (in[(1 - 1) + i * 15] == 1) {
			data[1 - 1] = '1';
			c_1++;
		}
		else data[1 - 1] = '0';

		if (in[(2 - 1) + i * 15] == 1) {
			data[2 - 1] = '1';
			c_2++;
		}
		else data[2 - 1] = '0';

		if (in[(3 - 1) + i * 15] == 1) {
			data[3 - 1] = '1';
			c_1++;
			c_2++; // new 
		}
		else data[3 - 1] = '0';

		if (in[(4 - 1) + i * 15] == 1) {
			data[4 - 1] = '1';
			c_4++;
		}
		else data[4 - 1] = '0';

		if (in[(5 - 1) + i * 15] == 1) {
			data[5 - 1] = '1';
			c_1++;
			c_4++;
		}
		else data[5 - 1] = '0';

		if (in[(6 - 1) + i * 15] == 1) {
			data[6 - 1] = '1';
			c_2++;
			c_4++;
		}
		else data[6 - 1] = '0';

		if (in[(7 - 1) + i * 15] == 1) {
			data[7 - 1] = '1';
			c_1++;
			c_2++;
			c_4++;
		}
		else data[7 - 1] = '0';

		if (in[(8 - 1) + i * 15] == 1) {
			data[8 - 1] = '1';
			c_8++;
			
		}
		else data[8 - 1] = '0';

		if (in[(9 - 1) + i * 15] == 1) {
			data[9 - 1] = '1';
			c_1++;
			c_8++;
			
		}
		else data[9 - 1] = '0';

		if (in[(10 - 1) + i * 15] == 1) {
			data[10 - 1] = '1';
			
			c_2++;
			c_8++;
		}
		else data[10 - 1] = '0';

		if (in[(11 - 1) + i * 15] == 1) {
			data[11 - 1] = '1';
			c_1++;
			c_2++;
			c_8++;
		}
		else data[11 - 1] = '0';

		if (in[(12 - 1) + i * 15] == 1) {
			data[12 - 1] = '1';
			c_4++;
			c_8++;
		}
		else data[12 - 1] = '0';

		if (in[(13 - 1) + i * 15] == 1) {
			data[13 - 1] = '1';
			c_1++;
			c_4++;
			c_8++;
		}
		else data[13 - 1] = '0';

		if (in[(14 - 1) + i * 15] == 1) {
			data[14 - 1] = '1';
			c_2++;
			c_4++;
			c_8++;
		}
		else data[14 - 1] = '0';

		if (in[(15 - 1) + i * 15] == 1) {
			data[15 - 1] = '1';
			c_1++;
			c_2++;
			c_4++;
			c_8++;
		}
		else data[15 - 1] = '0';
		data[15] = '\0';
		//printf("data is %s\n", data);
		checks_sum[0] = (c_8 % 2 == 1) ? 1 : 0;
		checks_sum[1] = (c_4 % 2 == 1) ? 1 : 0;
		checks_sum[2] = (c_2 % 2 == 1) ? 1 : 0;
		checks_sum[3] = (c_1 % 2 == 1) ? 1 : 0;
		check_index = checks_sum[3]  + (checks_sum[2] * 2 ) + (checks_sum[1] * 4) + (checks_sum[0] * 8);
		//printf("c_8 = %d, c_4 = %d , c_2 = %d, c_1 = %d\n", c_8, c_4, c_2, c_1);
		//printf("check_index is %d \n", check_index);
		
		if (check_index != 0) {
			*errors = *errors+1;
			data[check_index - 1] = (data[check_index - 1] == '1') ? '0' : '1';
			//printf("data with fix %s\n", data);
		}
		output[(1 - 1) + i * 11] = data[3-1];
		output[(2 - 1) + i * 11] = data[5-1];
		output[(3 - 1) + i * 11] = data[6-1];
		output[(4 - 1) + i * 11] = data[7-1];
		output[(5 - 1) + i * 11] = data[9-1];
		output[(6 - 1) + i * 11] = data[10 - 1];
		output[(7 - 1) + i * 11] = data[11 - 1];
		output[(8 - 1) + i * 11] = data[12 - 1];
		output[(9 - 1) + i * 11] = data[13 - 1];
		output[(10 - 1) + i * 11] = data[14 - 1];
		output[(11 - 1) + i * 11] = data[15 - 1];
		//printf("checks_sum is %s\n", checks_sum);
		
	}

	output[frames_num * 11] = '\0';
	//printf("output after hamming reverse is %s\n", output);

	int number_of_letters = (frames_num*11)/8;
	char* output_letters = (char*)calloc(number_of_letters + 1, sizeof(char));
	char temp[9];
	//char c;
	for (int j = 0; j < number_of_letters ; j++) { ///number_of_letters
		temp[0] = output[(1 - 1) + (8 * j)];
		temp[1] = output[(2 - 1) + (8 * j)];
		temp[2] = output[(3 - 1) + (8 * j)];
		temp[3] = output[(4 - 1) + (8 * j)];
		temp[4] = output[(5 - 1) + (8 * j)];
		temp[5] = output[(6 - 1) + (8 * j)];
		temp[6] = output[(7 - 1) + (8 * j)];
		temp[7] = output[(8 - 1) + (8 * j)];
		temp[8] = '\0';
		output_letters[j] = strtol(temp, 0, 2);
	}
	free(output);
	output_letters[number_of_letters] = '\0';
	//printf("output_letters after hamming reverse is %s\n", output_letters);
	return output_letters;
}


int create_noise(char* in, char* out, int len, time_t* seed, double p) {
	int change_bits = 0;
	char *out_bits = (char*)calloc(len + 1, sizeof(char));
	srand((unsigned)time(seed));
	for (int i = 0; i < len; i++) {
		if ((rand() / ((double)RAND_MAX)) < p) {
			out_bits[i] = (in[i] == 1) ? '0' : '1';
			change_bits++;
		}
		else out_bits[i] = (in[i]==1) ? '1':'0';
	}
	char temp[9];
	for (int j = 0; j < len/8; j++) { ///number_of_letters
		temp[0] = out_bits[(1 - 1) + (8 * j)];
		temp[1] = out_bits[(2 - 1) + (8 * j)];
		temp[2] = out_bits[(3 - 1) + (8 * j)];
		temp[3] = out_bits[(4 - 1) + (8 * j)];
		temp[4] = out_bits[(5 - 1) + (8 * j)];
		temp[5] = out_bits[(6 - 1) + (8 * j)];
		temp[6] = out_bits[(7 - 1) + (8 * j)];
		temp[7] = out_bits[(8 - 1) + (8 * j)];
		temp[8] = '\0';
		out[j] = strtol(temp, 0, 2);
	}
	out[len / 8] = '\0';
	free(out_bits);

	return change_bits;
}