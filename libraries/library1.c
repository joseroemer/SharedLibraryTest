#include "stdio.h"
#include "stdlib.h"
#include <time.h>

void greetings()
{
	printf("    Hello compiler...\n");
}

void printDateTime()
{
	int hours, minutes, seconds, day, month, year;

	time_t now;
	time(&now);
	printf("    today is %s\n", ctime(&now));
}

/*
void main()
{
	greetings();
	printDateTime();
}
*/
