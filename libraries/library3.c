#include "stdio.h"
#include "stdlib.h"
#include <time.h>
#include <string.h>

void runtimeError()
{
	int a=1,b=0;
	printf("division %d", a/b);
}

void main()
{
	runtimeError();
}
