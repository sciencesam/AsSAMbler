#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isReg(char *s);
int isOpcode(char *s);
int isLabel(char *s);
int addToRegisterList(char *s);
int fufillLabel();

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
		char *op;
		char *label;
		char *Dest;
		char *Src;

		int uin; //use input not
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


	//collect line by line and format
	char funExp[] = "\n\t\r ,"; //delimiters
	char *token; //token for words
	char line[100]; //temp storage for each line
	char *operation[3]; //temp operation storage
	int cCount = 2;
	while(fgets(line, sizeof line, fp)!=NULL){
		token = strtok(line, funExp);
		while(token != NULL){

			printf("|%s| ", token);

			//check for label
			if(isLabel(token)){
				//add to label database
			}
			//check for opcode
			if(isOpcode(token)){
				fufillLabel(); //any un-enumerated labels point here

			}
			//check for Rd
			if(cCount == 1 && isReg(token)){

			}
			//check for Rs or input
			if(cCount == 0){
				if(isReg(token)){

				}
				else {
					//Function using Input
				}

			}



			token = strtok(NULL, funExp);
		}
		printf("\n");
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
int fufillLabel(){

}