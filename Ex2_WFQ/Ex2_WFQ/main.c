#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include "queue_and_package.h"

#include <stdlib.h>
#include <string.h>

#define ADDRESS_LEN 16
#define LINE_SIZE 100

float check_for_weight(char* all_line);

void PrintOutput(Package* now_package, FILE* output, int rtime);
void extract_arguments(char* line, int* time, char* Sadd, int* Sport, char* Dadd, int* Dport, int* length, float* weight);
Package* read_and_insert_package(QUEUE** ptr_head, Package* new_package, int* flag_eof);
float UpdateRound(QUEUE* head, float prev_round_t, int rtime, float last_t_event);
void handle_arrival_event(QUEUE* head, int rtime);

int main() {
	QUEUE* head = NULL;
	int rtime = 0; // real time
	int flag_eof = 0;
	int arrive = 0; // 1 if arrive a packet at time rtime
	int empty_q = 1; //if 1 can be sent package, if 0 its occupied.
	int remaining_time = 0; /// remaining time of the current package
	FILE* input = stdin;
	FILE* output = stdout;
	Package* new_package =NULL;
	Package* now_package = NULL;
	
	//insert first package
	new_package = read_and_insert_package(&head, new_package, &flag_eof);
	do { // iterations of time

		if (empty_q == 0) { //checking the bus situation, update remaning time
			remaining_time = remaining_time - 1;
			if (remaining_time == -1) {
				empty_q = 1;
			}
		}
		while (flag_eof==0 && new_package->time == rtime) { //inserting new packets
				arrive = 1;
				new_package = read_and_insert_package(&head,new_package, &flag_eof);
		}
		
		if (arrive == 1) { // packets had arrived
			handle_arrival_event(head, rtime);
			arrive = 0;
		}

		if (empty_q == 1 && head != NULL) { // sending a new package to the bus
			now_package = GetPackageWithMinimumLast(head);
			if (now_package != NULL) {
				empty_q = 0;
				remaining_time = now_package->length - 1;
				PrintOutput(now_package, output, rtime);
			}
		}
		rtime++;
	} while (flag_eof == 0|| !AllEmpty(head) || empty_q==0);

	ReleaseAll(head,&head);
}

void PrintOutput(Package* now_package, FILE* output, int rtime)
{
	if (now_package->print_weight == 0) {
		fprintf(output, "%d: %d %s %d %s %d %d\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length);
	}
	else {
		fprintf(output, "%d: %d %s %d %s %d %d %.2f\n", rtime, now_package->time, now_package->Sadd, now_package->Sport, now_package->Dadd, now_package->Dport, now_package->length, now_package->weight);
	}
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

Package* read_and_insert_package(QUEUE** ptr_head, Package* new_package, int * flag_eof ) {
	char line[LINE_SIZE];
	int time;
	char Sadd[ADDRESS_LEN];
	int Sport;
	char Dadd[ADDRESS_LEN];
	int Dport;
	int length;
	float weight;
		
	if (new_package != NULL) {
		InsertNewPackage(ptr_head, new_package);
	}
		if (fgets(line, LINE_SIZE, stdin) == NULL) {
			*flag_eof = 1;
			return NULL;
		}
		extract_arguments(line, &time, Sadd, &Sport, Dadd, &Dport, &length, &weight);
		new_package = CreatePackage(time, Sadd, Sport, Dadd, Dport, length, weight, -1);
		return new_package;
	
}

void extract_arguments(char* line, int *time,char* Sadd,int*Sport,char* Dadd, int* Dport,int*length, float* weight) {
	float ret_val = check_for_weight(line);
	if (ret_val == 1.0) {
		*weight = -1;
		sscanf(line, "%d %s %d %s %d %d", time, Sadd, Sport, Dadd, Dport, length);
	}
	else {
		sscanf(line, "%d %s %d %s %d %d %f", time, Sadd, Sport, Dadd, Dport, length, weight);
	}
}

float UpdateRound(QUEUE* head,float prev_round_t, int rtime, float last_t_event) {
	float delta_t = rtime - last_t_event;
	float active_links_weight_t = SumActiveLinksWeights(head, prev_round_t);

	if (active_links_weight_t == 0) {
		return prev_round_t;
	}
	else {
		return prev_round_t + (delta_t / active_links_weight_t);
	}
}


void handle_arrival_event(QUEUE* head,int rtime) {
	bool next_event_is_arrival = false;
	static float prev_round_t = 0;
	static float last_t_event = 0;
	static float round_t = 0;
	float next_depart = -1;
	float x = 0;

	while (next_event_is_arrival == false) {
		round_t = UpdateRound(head, prev_round_t, rtime, last_t_event);

		// checking special case - if there is a package that ends before this one arrived - in GPS. 
		next_depart = GetNextDeparture(head, prev_round_t);
		if ((round_t > next_depart) && (next_depart != (float)-1)) {

			x = (next_depart - prev_round_t) * SumActiveLinksWeights(head, prev_round_t);
			last_t_event = last_t_event + x;
			prev_round_t = next_depart;
		}
		else {
			next_event_is_arrival = true;
		}
	}
	UpdateLast(head, round_t);
	last_t_event = rtime;
	prev_round_t = round_t;
}