#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include "queue_and_package.h"

#include <stdlib.h>
#include <string.h>

#define ADDRESS_LEN 16
#define LINE_SIZE 100

float check_for_weight(char* all_line);


int main() {
	QUEUE* head = NULL;
	int rtime = 0; // real time
	/// packet variables
	int time;
	char Sadd[ADDRESS_LEN];
	int Sport;
	char Dadd[ADDRESS_LEN];
	int Dport;
	int length;
	int flag_eof = 0;
	float weight = 0;
	float last;
	float last_t_event = 0;
	int arrive = 0; // 1 if arrive a packet at time rtime
	float round_t = 0;
	float active_links_weight_t = 0;
	float delta_t = 0;
	int empty_q = 1; //if 1 can be sent package, if 0 its occupied.
	int remaining_time = 0; /// remaining time of the current package
	float prev_round_t = 0;
	float x = 0;
	float next_depart=-1;

	FILE* input = stdin;
	FILE* output = stdout;
	
	Package new_package_obj;
	Package now_package_obj;
	Package* new_package = &new_package_obj;
	Package* now_package = &now_package_obj;
	

	char line[LINE_SIZE];
	fgets(line, LINE_SIZE, input); // return NULL if empty 

	weight = check_for_weight(line);
	if (weight == 1.0) {
		weight = -1;
		sscanf(line, "%d %s %d %s %d %d", &time, Sadd, &Sport, Dadd, &Dport, &length);
	}
	else {
		sscanf(line, "%d %s %d %s %d %d %f", &time, Sadd, &Sport, Dadd, &Dport, &length, &weight);
	}
	while (1) { // iterations of time

		if (empty_q == 0) { //checking the bus situation, update remaning time
			remaining_time = remaining_time - 1;
			if (remaining_time == -1) {
				empty_q = 1;
			}
		}
		while (1) { //inserting new packets
			if (flag_eof == 1) break;
			new_package = CreatePackage(time, Sadd, Sport, Dadd, Dport, length, weight, -1);
			if (time == rtime) {
				arrive = 1;
				InsertNewPackage(&head, new_package);
				if (fgets(line, LINE_SIZE, input) == NULL) {
					flag_eof = 1;
					break;
				}
				weight = check_for_weight(line);
				if (weight == 1.0) {
					weight = -1;
					sscanf(line, "%d %s %d %s %d %d", &time, Sadd, &Sport, Dadd, &Dport, &length);
				}
				else {
					sscanf(line, "%d %s %d %s %d %d %f", &time, Sadd, &Sport, Dadd, &Dport, &length, &weight);
				}
			}
			else break;

		}
		
		if (arrive == 1) { // packets had arrived
			int next_event_is_arrival = 0;

			while (next_event_is_arrival == 0) {
				/// here we are going to calculate roundt
				delta_t = rtime - last_t_event;
				active_links_weight_t = SumActiveLinksWeights(head, prev_round_t);
				if (active_links_weight_t == 0) {
					round_t = prev_round_t;
				}
				else {
					round_t = prev_round_t + (delta_t / active_links_weight_t);
				}
				///// checking special case - if there is a package that ends before this one arrived - in GPS. 
				next_depart = GetNextDeparture(head, prev_round_t);
				if ((round_t > next_depart) && (next_depart != (float)-1)) {

					x = (next_depart - prev_round_t) * active_links_weight_t;
					last_t_event = last_t_event + x;
					prev_round_t = next_depart;
				}
				else {
					next_event_is_arrival = 1;
				}
			}
			UpdateLast(head, round_t);
			last_t_event = rtime;
			prev_round_t = round_t;
		}

		if (empty_q == 1 && head != NULL) { // sending a new package to the bus
			now_package = GetPackageWithMinimumLast(head);
			if (now_package != NULL) {
				empty_q = 0;
				remaining_time = now_package->length - 1;
				if (now_package->print_weight == 0) {
					fprintf(output, "%d: %d %s %d %s %d %d\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length);
				}
				else {
					fprintf(output, "%d: %d %s %d %s %d %d %.2f\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length, now_package->weight);
				}
			}
		}

		if (flag_eof == 1 && AllEmpty(head) && empty_q == 1) break;//check if to exit program
		rtime++;
		arrive = 0;
	}

	ReleaseAll(head,&head);
	

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


