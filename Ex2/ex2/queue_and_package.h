#ifndef	QUEUE_AND_PACKAGE_H
#define	QUEUE_AND_PACKAGE_H

#include <stdbool.h>


typedef struct Package {
	int time;
	char Sadd[20];
	int Sport;
	char Dadd[20];
	int Dport;
	int length;
	float weight;//optional
	int remain_time_to_finish;
	struct Package* next;

} Package;


/*this struct defines the queue data structure*/
typedef struct Queue {
	int size;//the size of queue
	Package* head;//pointer to the first node in queue(first in priority)
	Package* tail;//pointer to the last node in queue(last in priority)
}QUEUE;


// Function Declarations -------------------------------------------------------

/*this function creats new queue
if succeed the function return a new pointer to an empty queue
if fails returns NULL*/
QUEUE* InitializeQueue();

/*this function return the 'last' of the first package in queue*/
/*input: pointer to queue*/
int LastOfTopPackage(QUEUE* queue);

/*this function push a new package the the end of queue*/
void Push(QUEUE* queue,//pointer to queue
	Package* package);// the new package that will be insert to the end of queue

//this function get a pointer to queue and check if it empty
//return true if it empty, otherwise return false.
bool Empty(QUEUE* queue);

/*this funtion release all the memory that allocated to the queue including the queue, and setting the the queue pointer to NULL
/*inputs: arg1 -pointer to the queue that need to be released
arg2- pointer to pointer of the same queue
example for using the function "DestroyQueue(queue_ptr,&queue_ptr)"*/
void DestroyQueue(QUEUE* ptr_queue, QUEUE** double_ptr_queue);


//this function creats a new package 
//if succeed it return pointer to new node, otherwise it return NULL
Package* CreatePackage(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight);


#endif