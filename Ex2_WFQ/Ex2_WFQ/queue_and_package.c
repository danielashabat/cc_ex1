#define _CRT_SECURE_NO_WARNINGS

#include "queue_and_package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




#define FUNCTION_FAILED 1



/*this function creats new queue
if succeed the function return a new pointer to an empty queue
if fails returns NULL*/
QUEUE* InitializeQueue();


/*this function return the 'last' of the first package in queue*/
/*input: pointer to queue*/
Package* LastOfTopPackage(QUEUE* queue);
/*this function push a new package the the end of queue*/
void Push(QUEUE* queue,//pointer to queue
	Package* package);// the new package that will be insert to the end of queue

//this function get a pointer to queue and check if it empty
//return true if it empty, otherwise return false.
bool Empty(QUEUE* queue);


/*this function return the pointer to the first package in queue*/
/*input: pointer to queue*/
Package* Pop(QUEUE* queue);

QUEUE* SearchQueue(QUEUE* head, char* Sadd, int Sport, char* Dadd, int Dport);

/*this funtion release all the memory that allocated to the queue including the queue, and setting the the queue pointer to NULL
/*inputs: arg1 -pointer to the queue that need to be released
arg2- pointer to pointer of the same queue
example for using the function "DestroyQueue(queue_ptr,&queue_ptr)"*/
void DestroyQueue(QUEUE* ptr_queue, QUEUE** double_ptr_queue);

void DestroyEmptyQueue(QUEUE** ptr_head, QUEUE* queue);
// Implementation -------------------------------------------------------

QUEUE* InitializeQueue() {
	QUEUE* queue = (QUEUE*)malloc(sizeof(QUEUE));
	if (queue == NULL) {
		return NULL;
	}

	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;
	queue->next = NULL;
	queue->weight = 1;

	return queue;
}

void Push(QUEUE* queue, Package* package) {

	if (package == NULL)
	{
		printf("Fatal error: package is NULL!\n");
		exit(FUNCTION_FAILED);//NEED TO TERMINATE PROGRAM
	}
	if (Empty(queue)) {
		queue->head = package;//the first node is head and tail
		queue->tail = package;
		queue->size++;//increase queue's size by 1
		strcpy(queue->Dadd,package->Dadd);
		strcpy(queue->Sadd,package->Sadd);
		queue->Dport = package->Dport;
		queue->Sport = package->Sport;
	}
	else {//if queue is not empty
		package->prev = queue->tail;
		(queue->tail)->next = package; //link the last node in the queue to the new node
		queue->tail = package;//updating the tail in queue to the new node
		queue->size++;//increase queue's size by 1
	}

}

Package* CreatePackage(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight, float last) {
	Package* package = NULL;
	package = (Package*)malloc(sizeof(Package));
	if (package == NULL) {
		printf("ERROR: malloc() failed\n");
		exit(1);
	}
	package->time = time;
	package->Sport = Sport;
	package->Dport = Dport;
	package->length = length;
	package->weight = weight;
	package->last = last;
	package->print_weight = 0;
	package->ignore = false;

	strcpy(package->Sadd, Sadd);
	strcpy(package->Dadd, Dadd);

	package->next = NULL;
	package->prev = NULL;
	return package;
}

Package* LastOfTopPackage(QUEUE* queue) {
	if (Empty(queue)) {
		return NULL;
	}
	Package* pack = queue->head;
	while (pack!=NULL) {
		if (pack->ignore == false) {
			return pack;
		}
		pack = pack->next;
	}

	return NULL;//not found

	
}

void DestroyQueue(QUEUE* ptr_queue, QUEUE** double_ptr_queue) {
	if (!Empty(ptr_queue)) {//if queue is not empty, release all nodes
		Package* curr_node = ptr_queue->head;
		Package* temp = NULL;

		while (curr_node != NULL) {//free all nodes in queue
			temp = curr_node;
			curr_node = curr_node->next;
			free(temp);
		}
	}
	free(ptr_queue);//free queue
	*double_ptr_queue = NULL; //setting queue pointer to NULL 
}


void ReleaseAll(QUEUE* ptr_queue, QUEUE** double_ptr_queue) {
	QUEUE* queue = ptr_queue;
	QUEUE* next_queue = NULL;
	while (queue != NULL) {
		next_queue = queue->next;
		DestroyQueue(queue, &queue);
		queue = next_queue;
	}
}


Package* Pop(QUEUE* queue) {
	if (Empty(queue)) {
		printf("ERROR: Pop function failed because queue is empty\n");
		exit(FUNCTION_FAILED);
	}
	//if not empty
	Package* first = queue->head;

	if (queue->size == 1) {//when there is only 1 node
		queue->tail = NULL;
	}

	queue->head = first->next;
	queue->size--;

	return first;
}

bool Empty(QUEUE* queue) {
	if (queue != NULL) {//check if the pointer is valid
		if (queue->size == 0)
			return true;
		else
			return false;
	}
	printf("ERROR:queue pointer is not valid!\n");
	return FUNCTION_FAILED;
}

bool AllEmpty(QUEUE *head) {
	QUEUE* queue = head;
	Package* pack = queue->head;
	while (queue != NULL) {

		while (pack != NULL) {
			if (pack->ignore==false) {
				
				return false;
			}
			pack = pack->next;
		}

		queue = queue->next;
	}
	return true;
}

void InsertNewPackage(QUEUE** ptr_head, Package* new_package) {

	QUEUE* first_queue = *ptr_head;
	QUEUE* match_queue = NULL;

	if (first_queue != NULL) {
		match_queue = SearchQueue(first_queue, new_package->Sadd, new_package->Sport, new_package->Dadd, new_package->Dport);
	}

	if (match_queue == NULL) { //if a matching queue is not found, need to create new one 
		match_queue = InitializeQueue();
		*ptr_head = match_queue; //update head to be the new queue
		match_queue->next = first_queue;
	}

	Push(match_queue, new_package);
	//update weight
	if (new_package->weight != -1) {//if there is special weight. update queue's weight
		match_queue->weight = new_package->weight;
		new_package->print_weight = 1;
	}
	else {
		new_package->weight = match_queue->weight; //if not take the queue's weight for package
	}
	

}

//search queue with the same source address ,source port ,destination address,destination port
//reruen value: if found the right queue it returns pointer to the queue, in not returns NULL
QUEUE* SearchQueue(QUEUE* head, char* Sadd, int Sport, char* Dadd, int Dport) {

	QUEUE* queue = head;

	while (queue != NULL) {
		

		if (strcmp(queue->Dadd, Dadd) == 0 \
			&& strcmp(queue->Sadd, Sadd) == 0\
			&& queue->Sport == Sport \
			&& queue->Dport == Dport) {//check if match

			return queue;
		}
		queue = queue->next;
	}

	return NULL;//matching queue not found
}

Package* GetPackageWithMinimumLast(QUEUE* head) {
	QUEUE* queue = head;
	Package* pack_with_minimum_last = NULL;
	Package* pack = NULL;
	float minimum_last = 0xFFFFFFF;
	float last_of_top_package;

	if (queue == NULL) {
		printf("ERROR: there is no packages in queues\n terminate program\n");
		exit(FUNCTION_FAILED);
	}

	while (queue != NULL) {
		
			pack = LastOfTopPackage(queue);
			if (pack != NULL) {
				last_of_top_package = pack->last;
				if (last_of_top_package <= minimum_last) {
					pack_with_minimum_last = pack;
					minimum_last = last_of_top_package;
				}
			}
		
		queue = queue->next;
	}
	if (pack_with_minimum_last != NULL) {
		pack_with_minimum_last->ignore = true;
	}
	return pack_with_minimum_last;
}



void RemovPackageFromQueue(QUEUE * head, Package* package) {
	QUEUE* queue = head;
	Package* pack;
	QUEUE*  match_queue=NULL;

	if (queue == NULL) {
		printf("ERROR: there is no packages in queues\n terminate program\n");
		exit(FUNCTION_FAILED);
	}

	while (queue != NULL&& match_queue ==NULL) {
		pack = queue->head;
		while (pack != NULL) {
			if (pack == package) {
				match_queue = queue;
				break;
			}
			pack = pack->next;
		}
		queue = queue->next;
	}
	if (match_queue == NULL) {
		printf("ERROR: package not found in any queue\n terminate program\n");
		exit(FUNCTION_FAILED);
	}
	match_queue->size--;//decrease size of queue
}

void DestroyEmptyQueue(QUEUE** ptr_head, QUEUE* queue) {
	if (!Empty(queue)) {
		printf("ERROR: queue is not empty!\n");
		exit(FUNCTION_FAILED);
	}

	QUEUE* prev_queue = NULL;
	QUEUE* cur_queue = *ptr_head;

	while (cur_queue != NULL) {
		if (cur_queue == queue) {
			break;
		}
		prev_queue = cur_queue;
		cur_queue = cur_queue->next;
	}

	if (cur_queue == NULL) {
		printf("ERROR: the queue not found in list!\n");
		exit(FUNCTION_FAILED);
	}

	if (prev_queue != NULL) {
		prev_queue->next = cur_queue->next;
	}
	else {
		*ptr_head = cur_queue->next; //if the queue is the head, change head to be next queue 
	}

	free(cur_queue);//free queue 
}


void PrintQueues(QUEUE* head) {
	QUEUE* queue = head;
	Package* package = NULL;
	int i = 0;
	if (queue == NULL) {
		printf("[]\n");
		return;
	}

	while (queue != NULL) {
		printf("queue %d [weight: %.2f] :  ", i, queue->weight);
		i++;
		package = queue->head;
		for (int k = 0; k < queue->size; k++) {
			printf(" %f %d-> ", package->last, package->time);
			package = package->next;
		}
		printf("\n");
		queue = queue->next;
	}
}

float SumActiveLinksWeights(QUEUE* head, float round_t) {
	QUEUE* queue = head;
	float sum = 0;
	Package* pack;

	while (queue != NULL) {
		pack = queue->head;
		while (pack != NULL) {
			if (round_t < pack->last) {
				sum = sum + pack->weight;
				break;
			}
			pack = pack->next;
		}
		
		queue = queue->next;
	}
	return sum;
}

float GetPreviousPackageLast(Package* package) {
	Package* prev_package = package->prev;

	if (prev_package == NULL) {
		return -1;
	}
	return prev_package->last;
}

void UpdateLast(QUEUE* head, float round_t) {

	QUEUE* queue = head;
	Package* pack;
	float prev_last = 0;
	while (queue != NULL) {
		pack = queue->head;
		while (pack != NULL) {
			if (pack->last == -1) {
				//calculates the last
				prev_last = GetPreviousPackageLast(pack);
				if ((prev_last < round_t )|| (prev_last==-1)) {
					pack->last = round_t + (pack->length / pack->weight);
				}
				else {
					pack->last = prev_last + (pack->length / pack->weight);
				}
			}
			pack = pack->next;
		}
		queue = queue->next;
	}

}

//this function return the virtual time of the next departure package, if there is no packages to departure it returns -1;
float GetNextDeparture(QUEUE* head, float round_t) {
	QUEUE* queue = head;
	Package* pack;
	float minimum_last = -1;

	if (queue == NULL) {
		printf("ERROR: there is no packages in queues\n terminate program\n");
		exit(FUNCTION_FAILED);
	}

	while (queue != NULL) {
		pack = queue->head;

		while (pack != NULL) {
			//initialize minimun last
			if (round_t < pack->last && minimum_last==-1 && (pack->last != -1)) {
				minimum_last = pack->last;
				break;
			}
			//check if package is with minimum last
			if (round_t < pack->last && pack->last<= minimum_last && (pack->last != -1)) {
				minimum_last = pack->last;
				break;
			}
			pack = pack->next;
		}

		queue = queue->next;
	}
	return minimum_last;
}