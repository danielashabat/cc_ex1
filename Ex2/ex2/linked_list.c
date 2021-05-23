#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


///internal function declarations:

void insert_first_package(Package* Package, LinkedList* list);


Package* get_package_by_ID(int ID, LinkedList* list) {

		Package* package = list->first;

		for (int i = 0; i < list->num_of_packages; i++) {
			if (package->ID == ID) {
				break;
			}
			package = package->next;
			return package;
		}
	
		if (package == NULL) {
			printf("package ID not exist\n finish program\n");
			terminate_list(list);
			exit(1);
		}
}


LinkedList* initialize_linked_list() {
	LinkedList* list = NULL;
	list = (LinkedList*)malloc(sizeof(LinkedList));
	if (list == NULL) {
		printf("ERROR: malloc() failed\n");
		exit(1);
	}

	list->first = NULL;
	list->last = NULL;
	list->num_of_packages = 0;
	return list;
}



Package* create_package(int time, char* Sadd, int Sport, char* Dadd, int Dport, int length, float weight) {
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

	strcpy(package->Sadd,Sadd);
	strcpy(package->Dadd,Dadd);
	
	package->next = NULL;

	//assign an uniqify ID for every package
	static ID = 0;
	package->ID = ID; 
	ID++;
	return package;
}

void insert_first_package(Package* Package, LinkedList* list) {
	list->first = Package;
	list->last = Package;
	list->num_of_packages = 1;
}


void add_to_end_of_list(Package* new_package, LinkedList* list) {
	if (list->num_of_packages == 0) {
		insert_first_package(new_package, list);
	}
	else {
		list->last->next = new_package;
		list->last = new_package;
		list->num_of_packages++;
	}
}


void terminate_list(LinkedList* list) {
	Package* package = list->first;
	Package* temp = NULL;
	while (list->num_of_packages != 0) {
		temp = package->next;
		free(package);
		package = temp;
		list->num_of_packages--;
	}
	free(list);
}

void print_list(LinkedList* list) {
	Package* package = NULL;
	int i = 0;
	if (list->num_of_packages == 0) {
		printf("[]\n");
		return;
	}
	package = list->first;
	printf("[");

	for (i = 0; i < list->num_of_packages; i++) {
		printf("package ID: %d,\n ", package->ID );
		package = package->next;
	}
	printf("]\n");
}



int delete_package_from_list_by_ID(int ID, LinkedList* list) {
	Package* package = list->first;
	Package* prev_package = NULL;


	for (int i = 0; i < list->num_of_packages; i++) {
		if (package->ID == ID) {
			break;
		}
		prev_package = package;
		package = package->next;
	}

	if (package != NULL) {
		if (prev_package != NULL) {
			prev_package->next= package->next;
		}
		else { 
			list->first = package->next;
		}

		free(package);
		list->num_of_packages--;
		if (list->num_of_packages <= 1) {
			list->last = package->next;
		}
		return 0;
	}

	return 1;
}

//int delete_element_index_from_list(int index, Linked_List* list) {
//	int i = 0;
//	Node* node = NULL;
//	if (index >= list->num_of_nodes || index < 0) {
//		terminate_list(list);
//		return ERROR_INVALID_INDEX;
//	}
//	if (list->num_of_nodes == 1) {
//		free(list->first);
//		list->first = NULL;
//		list->last = NULL;
//		list->num_of_nodes = 0;
//		return 0;
//	}
//	if (index == 0) {
//		list->first = list->first->next;
//		free(list->first->prev);
//		list->first->prev = NULL;
//		list->num_of_nodes--;
//		return 0;
//	}
//	if (index == list->num_of_nodes - 1) {
//		list->last = list->last->prev;
//		free(list->last->next);
//		list->last->next = NULL;
//		list->num_of_nodes--;
//		return 0;
//	}
//	node = list->first;
//	for (i = 0; i < index; i++) {
//		node = node->next;
//	}
//	node->prev->next = node->next;
//	node->next->prev = node->prev;
//	free(node);
//	list->num_of_nodes--;
//	return 0;
//}