/*
NAME:
DATE:
DESCRIPTION:
SAMPLE INPUT:
SAMPLE OUTPUT:
*/
#include "header.h"
int error_flag;
int main(int argc,char* argv[])
{
	if(argc < 2)
	{
		printf("Usage: ./a.out <.c file>\n");
		return 0;
	}
	char filename[50];
	strcpy(filename,argv[1]);
	if(!strstr(filename,".c"))
	{
		printf("Please pass <.c file>\n");
		return 0;
	}
	FILE* fptr = fopen(filename,"r");
	printf("Open : %s : ",filename);
	if(fptr)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILED\n");
		return 0;
	}
	printf("Parsing : %s : Started\n",filename);

	lexical_analyze(fptr);
	fclose(fptr);
	if(error_flag)
	{
	printf("Tokens cannot be generated.\n");;
	}
	else
	{
		printf("Tokens generated.\n");
	}
	return 0;
}
