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
	float active_links_weight_t = 0;
	int delta_t = 0;
	int empty_q = 1; //if 1 can be sent package, if 0 its occupied.
	int remaining_time = 0; /// remaining time of the current package

	Sadd = (char*)malloc(16*sizeof(char));
	Dadd = (char*)malloc(16 * sizeof(char));
	FILE* input = NULL;
	FILE* output = NULL;
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	Package* new_package= (Package*)malloc(sizeof(Package));
	Package* now_package = (Package*)malloc(sizeof(Package));
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
	while (1) { // iterations of time
		if (remaining_time > 0) { // check about the bus
			remaining_time = remaining_time - 1;
			if (remaining_time == 0) { //the package done
				empty_q = 1;
				//printf("queue before pop\n");
				//PrintQueues(head);
				PopPackageWithMinimumLast(&head);
				printf("queue after pop\n");
				PrintQueues(head);
			}
		}

		while (1) { //inserting new packets
			
			if (2689 == rtime) {
				PrintQueues(head);
			}
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
			//printf("queue after inserting packets\n");
			//PrintQueues(head);
			
		}
		if (flag_eof == 1) break;
		if (arrive == 1) { // packets had arrived
			/// here we are going to calculate roundt
			delta_t = rtime - last_t_arrive;
			if (active_links_weight_t == 0) {
				round_t = round_t;
			}
			else {
				round_t = round_t + (delta_t / active_links_weight_t);
			}
			UpdateLast(head, round_t);
			printf("update last\n");
			PrintQueues(head);
			if (empty_q == 1) {
				now_package = GetPackageWithMinimumLast(head);
				empty_q = 0;
				remaining_time = now_package->length;
				if (now_package->weight == 1) {
					fprintf(output, "%d: %d %s %d %s %d %d\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length);
				}
				else {
					fprintf(output, "%d: %d %s %d %s %d %d %f\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length, now_package->weight);
				}
			}
			last_t_arrive = rtime;
			active_links_weight_t = SumActiveLinksWeights(head);
		}
		rtime++;
		arrive = 0;
	}

	//sorry Mor, commented all your work...



	//Package* pack1 =  CreatePackage(0, "source", 0, "dest" , 0, 100, 1,-1);
	//InsertNewPackage(&head, pack1);

	//Package*  new_package = CreatePackage(0, "source2", 0, "dest", 0, 50, 1, -1);
	//InsertNewPackage(&head, new_package);

	//new_package = CreatePackage(0, "source2", 0, "dest", 0, 20, 1, -1);
	//InsertNewPackage(&head, new_package);

	//PrintQueues(head);
	//UpdateLast(head, round_t);
	//PrintQueues(head);
	////pop all packages
	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);

	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);
	//int sum = SumActiveLinksWeights(head);
	//printf("the sum of active links: %d\n", sum);
	//PrintQueues(head);

	//new_package = GetPackageWithMinimumLast(&head);
	//printf("pop package with minimum last : %f\n", new_package->last);
	//sum = SumActiveLinksWeights(head);
	//printf("the sum of active links: %d\n", sum);
	PrintQueues(head);

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


