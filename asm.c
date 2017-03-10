#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isReg(char *s);
int isOpcode(char *s);
int isLabel(char *s);
int isComment(char *s);

int addToRegisterList(char *s);
int addToOpcodeList(char *s)
int fufillLabel(char *s, int opCount);

int main(int argc, char **argv){
	//check arguments
	if(argc < 2){
		printf("Missing Source File\n");
		return 1;
	}
	//open input and output files
	FILE *fp, *ofp;
	char *inputFilename = argv[1];
	char * outputFilename;
	if(argc > 2) {
		outputFilename = argv[2];
	}
	else {
		printf("Missing Output Filename, using out.sam\n");
		outputFilename = "out.sam";
	}
	fp = fopen(inputFilename, "r");
	if(fp == NULL){
		printf("Cannot Open File %s", inputFilename);
		return 1;
	}
	ofp = fopen(outputFilename, "w");
	if(ofp == NULL){
		printf("Cannot Output to %s", outputFilename);
		return 1;
	}
	//define datatypes
	struct {
		char *name;
		int line;
	} labels[100];
	int nLabel = 0;

	struct {
		char *name;
		int location;
	} registers[100];
	int nReg = 0;


	struct {
		char *name;
		char *label;
		char *Dest;
		char *Src;

		int ui; //use input
		int we; //write enable

		int opcode;
		// Rd and Rs for 
		int Rd;
		int Rs;
		//label address for branch
		int Addr;

		int hex; //final output

	} operations[100];
	int nOps = 0;

	typedef char * string;
	//collect line by line and format
	char funExp[] = "\n\t\r ,"; //delimiters
	char *token; //token for words
	char line[100]; //temp storage for each line
	string operation[3]; //temp operation storage
	int cCount = 2;
	while(fgets(line, sizeof line, fp)!=NULL){
		token = strtok(line, funExp);
		cCount = 2;
		while(token != NULL && cCount >= 0 ){
			printf("|%s| ", token);
			//Copy each operation to list
			strcpy(operation[cCount], token);
			cCount--;

			token = strtok(NULL, funExp);
		}
		printf("\n");
		if(isLabel(operation[2])){
			printf("%s is a Label\n", operation[2]);
			//add to label database
		}
		if(isOpcode(operation[2])){
			printf("%s is an opcode\n", operation[2]);
			strcpy(operations[nOps].name , operation[2]);
			if(isReg(operation[1])){
				printf("%s is Rd\n", operation[1]);
				strcpy(operations[nOps].Dest, operation[1]);
				if(isReg(operation[0])){
					printf("%s is Rs\n", operation[0]);
					strcpy(operations[nOps].Src, operation[0]);
					operations[nOps].ui = 0;
				}
				else {
					printf("%s uses Input\n", operation[2]);
					operations[nOps].ui = 1;
				}
			}
			else {
				printf("%s is a branch to %s\n", operation[2], operation[1]);
				strcpy(operations[nOps].label, operation[1]);
				operations[nOps].we = 0;

		}

	} //end of input


	return 0;
}

int isReg(char *s){
	//All registers are labeled with an 'R'
	if(toupper(s[0]) == 'R') return 1;
	else return 0;
}
int isOpcode(char *s){
	//look through opcode dictionary for a match
}
int isLabel(char *s){
	//The last character of a label is ':'
	if(s[strlen(s)-1] == ":") return 1;
	else return 0;
}
int addToRegisterList(char *s){

}
int fufillLabel(char *s, int opCount){

}
int isComment(char *s){
	if(s[0] == ';') return 1;
	else return 0;
}