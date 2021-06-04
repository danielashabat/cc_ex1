#ifndef	LINKED_LIST_H
#define	LINKED_LIST_H	

#include "queue_and_package.h"



typedef struct Linked_List {
	int num_of_packages;
	struct Package* first;
	struct Package* last;
} LinkedList;

LinkedList* initialize_linked_list();

Package* get_package_by_ID(int ID, LinkedList* list);

void add_to_end_of_list(Package* new_package, LinkedList* list);


void terminate_list(LinkedList* list);

void print_list(LinkedList* list);

Package* create_package(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight);

int delete_package_from_list_by_ID(int ID, LinkedList* list);

#endif
