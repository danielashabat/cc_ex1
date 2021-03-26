#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>




long  FileLen(FILE* fileptr) {
	long filelen;
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file
	return filelen;
}

int main() {
	int filelen;
	int newfilelen = 0;
	char* buffer;
	FILE* fileptr = NULL;
	FILE* newfileptr = NULL;
	int extra_bits = 0;

	fileptr = fopen("jpg_file.jpg", "rb");  // Open the file in binary mode
	if (fileptr == NULL) {
		fprintf(stderr, "-ERROR- fopen() failed\n");
		return 1;
	}
	filelen = FileLen(fileptr);
	if (filelen == 0) {
		fprintf(stderr, "-ERROR- the file is empty");
	}
	
	extra_bits = 11- (filelen % 11);
	fprintf(stderr, "filelen is:%d, extra bits is: %d\n", filelen, extra_bits);
	newfilelen = extra_bits + filelen;
	buffer = (char*)malloc(newfilelen * sizeof(char));
	
	fread(buffer, 1, filelen, fileptr); // Read in the entire file
	for (int i = filelen; i < newfilelen; i++) {
		buffer[i] = 0;
	}
	newfileptr = fopen("test_jpg.jpg", "wb");
	fwrite(buffer, 1, newfilelen, newfileptr);
	fclose(newfileptr);
	fclose(fileptr);


	free(buffer);
	return;
}

