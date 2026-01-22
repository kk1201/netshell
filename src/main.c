#include <stdio.h>

int testfunc(int c)
{
	return 3;
}

int main(int argc, char** argv)
{
	printf("Starting up project netshell\n");
	printf("%d\n", testfunc(1));
}

