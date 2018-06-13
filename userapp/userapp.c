#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#define DEVICE "/dev/lcd_module"

int main()
{
	int i;
	FILE *fptr;
	char ch, write_buff[100], read_buff[100];

	fptr = fopen(DEVICE, "w");

	if (fptr == NULL){
		printf("file is either locked or does not exist\n");
		exit(-1);
	}

	printf("write information you want to be writen to device: \n");

	printf("enter data: ");
	scanf("%[^\n]", write_buff);

	fwrite(&write_buff, sizeof(write_buff), 1, fptr);

	fclose(fptr);

	return 0;
}
