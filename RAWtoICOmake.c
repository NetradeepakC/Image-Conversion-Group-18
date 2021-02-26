#include<stdio.h>
#include<string.h>
#include"TifftoRAW.h"
#include"RAWtoIco.h"
int main()
{
	FILE *fp2;
	char str[67];
	printf("Enter file name: ");
	scanf("%s",str);
	fp2 = fopen(str,"rb");
	ReadtoRAW(fp2);
	fclose(fp2);
	rawtoico();
	return 0;
}
