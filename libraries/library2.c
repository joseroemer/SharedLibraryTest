#include "stdio.h"
#include "stdlib.h"
#include <time.h>
#include <string.h>

void displayLinuxVersion()
{
	printf("Displaying LINUX version\n");
	char command[50];
	strcpy(command,"lsb_release -d");
	system(command);
}	
/*
void main()
{
	displayLinuxVersion();
}
*/