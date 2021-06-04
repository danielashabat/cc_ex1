#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "queue_and_package.h"


#define FUNCTION_FAILED 1



/*this function creats new queue
if succeed the function return a new pointer to an empty queue
if fails returns NULL*/
QUEUE* InitializeQueue();


/*this function return the 'last' of the first package in queue*/
/*input: pointer to queue*/
float LastOfTopPackage(QUEUE* queue);

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
	}
	else {//if queue is not empty
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

	strcpy(package->Sadd, Sadd);
	strcpy(package->Dadd, Dadd);

	package->next = NULL;
	return package;
}

float LastOfTopPackage(QUEUE* queue) {
	if (Empty(queue)) {
		printf("ERROR: Top function failed because queue is empty\n");
		exit(FUNCTION_FAILED);
	}
	//if not empty
	return (queue->head)->last;
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

void InsertNewPackage(QUEUE ** ptr_head, Package* new_package) {

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
	
}

//search queue with the same source address ,source port ,destination address,destination port
//reruen value: if found the right queue it returns pointer to the queue, in not returns NULL
QUEUE* SearchQueue(QUEUE* head, char* Sadd, int Sport, char* Dadd, int Dport) {
	
	QUEUE* queue = head;

	while (queue != NULL) {
		Package* package_in_queue = queue->head;

		if (strcmp(package_in_queue->Dadd,Dadd)==0 \
			&& strcmp(package_in_queue->Sadd, Sadd)==0\
			&& package_in_queue->Sport == Sport \
			&& package_in_queue->Dport == Dport) {//check if match
			
			return queue;
		}
		queue = queue->next;
	}

	return NULL;//matching queue not found
}


Package* GetPackageWithMinimumLast(QUEUE** ptr_head) {
	QUEUE* queue = *ptr_head;
	QUEUE* queue_with_minimum_last = queue;
	float minimum_last = queue_with_minimum_last->head->last;

	if (queue == NULL) {
		printf("ERROR: there is no packages in queues\n terminate program\n");
		exit(FUNCTION_FAILED);
	}

	while (queue != NULL) {
		if (LastOfTopPackage(queue) < minimum_last) {
			queue_with_minimum_last = queue;
			minimum_last = LastOfTopPackage(queue);
		}
		queue = queue->next;
	}
	Package* package_with_minimum_last = Pop(queue_with_minimum_last);

	if (queue_with_minimum_last->size == 0) {// check if it was the last package in queue 
		DestroyEmptyQueue(ptr_head, queue_with_minimum_last);
	}

	return package_with_minimum_last;

}

void DestroyEmptyQueue(QUEUE** ptr_head, QUEUE* queue) {
	if (!Empty(queue)) {
		printf("ERROR: queue is not empty!\n");
		exit(FUNCTION_FAILED);
	}

	QUEUE* prev_queue = NULL;
	QUEUE * cur_queue = *ptr_head;

	while (cur_queue !=NULL){
		if (cur_queue == queue) {
			break;
		}
		prev_queue = cur_queue;
		cur_queue = cur_queue->next;
	}

	if (cur_queue==NULL) {
		printf("ERROR: the queue not found in list!\n");
		exit(FUNCTION_FAILED);
	}

	if (prev_queue != NULL) {
		prev_queue->next = cur_queue->next;
	}
	else {
		*ptr_head= cur_queue->next; //if the queue is the head, change head to be next queue 
	}

	free(cur_queue);//free queue 
}


void PrintQueues(QUEUE* head) {
	QUEUE* queue = head;
	Package* package = NULL;
	int i = 0;
	if (queue ==NULL) {
		printf("[]\n");
		return;
	}

	while (queue != NULL) {
		printf("queue %d :  ", i);
		i++;
		package = queue->head;
		for (int k = 0; k < queue->size; k++) {
			printf(" %f -> ", package->last);
			package = package->next;
		}
		printf("\n");
		queue = queue->next;
	}
}