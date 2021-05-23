#include <stdio.h>
#include "linked_list.h"

int main() {
	LinkedList* list = NULL;
	list= initialize_linked_list();

	//creates new package and add to list
	Package* package=  create_package(0, "source", 0, "dest" , 0, 100, 1);
	add_to_end_of_list(package, list);
	package = create_package(0, "source", 0, "dest", 0, 100, 1);
	add_to_end_of_list(package, list);

	print_list(list);
	package =get_package_by_ID(0,list);
	delete_package_from_list_by_ID(package->ID, list);
	print_list(list);


	terminate_list(list);


}