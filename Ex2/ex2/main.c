#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "queue_and_package.h"

#include <stdlib.h>
#include <string.h>


float check_for_weight(char* all_line);

int main(int argc, char* argv[]) {
	QUEUE* head = NULL;
	int rtime=0; // real time
	/// packet variables
	int time;
	char* Sadd=NULL;
	int Sport;
	char* Dadd=NULL;
	int Dport;
	int length;
	int flag_eof = 0;
	float weight=0;
	float last;
	int last_t_arrive = 0;
	int arrive = 0; // 1 if arrive a packet at time rtime
	float round_t=0;

	Sadd = (char*)malloc(16*sizeof(char));
	Dadd = (char*)malloc(16 * sizeof(char));
	FILE* input = NULL;
	input = fopen(argv[1], "r");
	Package* new_package= (Package*)malloc(sizeof(Package));

	/// reading first line from file
	//fscanf(input, "%[^\n]", all_line);
	const size_t line_size = 100;
	char* line = malloc(line_size);
	fgets(line, line_size, input); // return NULL if empty 

	weight = check_for_weight(line);
	if (weight == 1.0) {
		sscanf(line, "%d %s %d %s %d %d", &time, Sadd, &Sport, Dadd, &Dport, &length);
	}
	else {
		sscanf(line, "%d %s %d %s %d %d %f", &time, Sadd, &Sport, Dadd, &Dport, &length, &weight);
	}
	while (1) {
		while (1) {
			
			new_package = CreatePackage(time, Sadd, Sport, Dadd, Dport, length, weight, -1);
			if (time == rtime) {
				arrive = 1;
				InsertNewPackage(&head, new_package);
				if (fgets(line, line_size, input) == NULL) {
					flag_eof = 1;
					break;
				}
				weight = check_for_weight(line);
				if (weight == 1.0) {
					sscanf(line, "%d %s %d %s %d %d", &time, Sadd, &Sport, Dadd, &Dport, &length);
				}
				else {
					sscanf(line, "%d %s %d %s %d %d %f", &time, Sadd, &Sport, Dadd, &Dport, &length, &weight);
				}
			}
			else break;
		}
		if (flag_eof == 1) break;
		if (arrive == 1) { // packets had arrived
			/// here we are going to calculate roundt
			last_t_arrive = rtime;
		}
		rtime++;
		arrive = 0;
	}

	//Package* new_package =  CreatePackage(0, "source", 0, "dest" , 0, 100, 1,7);
	//InsertNewPackage(&head, new_package);

	//new_package = CreatePackage(0, "source2", 0, "dest", 0, 100, 1, 5);
	//InsertNewPackage(&head, new_package);

	//new_package = CreatePackage(0, "source2", 0, "dest", 0, 100, 1, 3);
	//InsertNewPackage(&head, new_package);

	//PrintQueues(head);

	////pop all packages
	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);

	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);
	PrintQueues(head);

	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);

	//PrintQueues(head);

	//fscanf("%d %d %d\n",  );

}

float check_for_weight(char* all_line) {
	int count = 0;
	int len_line = strlen(all_line);
	for (int i = 0; i < len_line; i++) {
		if (all_line[i] == ' ') count++;

	}
	if (count == 5) return 1.0;
	else return 0.0;
}


