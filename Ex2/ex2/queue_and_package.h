#ifndef	QUEUE_AND_PACKAGE_H
#define	QUEUE_AND_PACKAGE_H


typedef struct Package {
	int time;
	char Sadd[20];
	int Sport;
	char Dadd[20];
	int Dport;
	int length;
	float weight;//optional
	float last;
	struct Package* next;

} Package;


/*this struct defines the queue data structure*/
typedef struct Queue {
	int size;//the size of queue
	Package* head;//pointer to the first node in queue(first in priority)
	Package* tail;//pointer to the last node in queue(last in priority)
	struct Queue* next;
}QUEUE;


// Function Declarations -------------------------------------------------------


//this function creats a new package 
//if succeed it return pointer to new node, otherwise it return NULL
Package* CreatePackage(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight, float last);


/*this function search for the right queue for the new package and insert it to the  queue, if no queue found it will create new queue and insert the package*/
void InsertNewPackage(QUEUE ** ptr_head, Package* new_package);

/*this function returns the package with minimum last*/
Package* GetPackageWithMinimumLast(QUEUE** ptr_head);

void PrintQueues(QUEUE* head);


///New functions: 
/// function that sums the weights of the active links.
/// function that returning the last of the p(i-1)
/// function that going over the queues and calling last calculating function. 
#endif