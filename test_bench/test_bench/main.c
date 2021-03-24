#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


void main() {
	FILE* ptr = NULL;
	ptr= fopen("test_file.txt", "w");

	for (int i = 0; i < 1650; i++) {
		fprintf(ptr, "a");
	}
	return;
}