#include <stdio.h>
#include "queue_and_package.h"

int main() {
	QUEUE* head = NULL;


	Package* new_package =  CreatePackage(0, "source", 0, "dest" , 0, 100, 1,7);
	InsertNewPackage(&head, new_package);

	new_package = CreatePackage(0, "source2", 0, "dest", 0, 100, 1, 5);
	InsertNewPackage(&head, new_package);

	new_package = CreatePackage(0, "source2", 0, "dest", 0, 100, 1, 3);
	InsertNewPackage(&head, new_package);

	PrintQueues(head);

	//pop all packages
	new_package = GetPackageWithMinimumLast(&head);
	printf("pop package with minimum last : %f\n", new_package->last);

	new_package = GetPackageWithMinimumLast(&head);
	printf("pop package with minimum last : %f\n", new_package->last);
	PrintQueues(head);

	new_package = GetPackageWithMinimumLast(&head);
	printf("pop package with minimum last : %f\n", new_package->last);

	PrintQueues(head);
}