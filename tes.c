#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* toHex(int numberB10){
	//returns the base 10 number in the base 16//
	const float log16 = 1.204;
	const int numChars = ceil((float) log(numberB10)/log(16))-1;
	static char theWord[3]={0x00};
	static char *hex;
	hex = malloc(sizeof(char)*(numChars));
	printf("Numchars = %d Number= %d\n", numChars, numberB10);
	int j;
	int theNumber;
	for(j = numChars; j>=0; j--){
		//printf("r\n");
		theNumber = numberB10%16;
		if(theNumber <=9) hex[j] = '0'+theNumber;
		if(theNumber >9) hex[j] = 'A'+theNumber-10;
		numberB10 = numberB10/16;  
	}
	//if(theWord[0] == '0') theWord[0] = ' ';
	return hex;
}



int main(int argc, char **argv){
	char *pt;
	int ret;
	ret = strtol(argv[1], &pt, 10);
	
	printf("%s\n", toHex(ret));
	const char *q = toHex(ret);
	char news[10] = {0x00};
	int sam = 0;
	while(q[sam] != NULL){
		news[sam] = q[sam];
		sam++;
	}
	printf("%s %s \n", q, news);
	return 0;
}