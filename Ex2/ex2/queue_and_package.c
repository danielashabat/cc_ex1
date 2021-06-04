#include <stdlib.h>
#include "queue_and_package.h"


#define FUNCTION_FAILED -1

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

Package* CreatePackage(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight) {
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
	package->remain_time_to_finish = length;

	strcpy(package->Sadd, Sadd);
	strcpy(package->Dadd, Dadd);

	package->next = NULL;
	return package;
}

int LastOfTopPackage(QUEUE* queue) {
	if (Empty(queue)) {
		printf("ERROR: Top function failed because queue is empty\n");
		return FUNCTION_FAILED;
	}
	//if not empty
	return (queue->head)->offset;
}



