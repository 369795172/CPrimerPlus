/* 6.16.6.c */
#include <stdio.h>
#include <string.h>
#define LEN 8
int main(void)
{
	int index;
	int number[LEN];
	
	printf("Enter numbers: \n");
	for (index=0; index < LEN; index++)
	{
		scanf("%d", &number[index]);
	}
	printf("The reversed number is: \n");
	for (index=LEN-1; index >= 0; index--)
		printf("%d ", number[index]);
	printf("\n");
	return 0;
}
