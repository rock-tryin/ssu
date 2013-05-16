#include<stdlib.h>
#include<stdio.h>

void main(void)
{
	char * workspace;
	workspace=getenv("ANDROID_PROPERTY_WORKSPACE");
	printf("workspace: %s\n",workspace);
}
