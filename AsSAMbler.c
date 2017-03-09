//Sam Stone EECE@Binghamton 3/8/2017

/*
This is a simple assembler for EECE@Binghamton. Compile with MAKEFILE.
To Build:
	make build

To Run: 
	AsSAMbler <input file> <optional output file>

Here is a Table of commands:

*** TODO: list of commands ***

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOPS 100 //PC only holds 16 anyway

//checks for a label in a string
int isLabel(){
	return 0;
}
//add information about line
int classifyOperation(){
	return 0;
}
int isOp(char *s){
	#define NUMOPS 20
	char ops[NUMOPS][5] = {
		"MOV","EOR","AND","OR","ADD",
		 "SUB","LSR", "MUL","JMP", "BCC",
		  "BCS", "BZC", "BZS", "BNC", "BNS",
		   "CLR", "INC", "CP", "BRMI", "COM"};
	int i;
	for(i=0; i<NUMOPS; i++){
		if(strcmp(ops[i], s) == 0) return 1;
	}
	return 0;
}
int main(int argc, char **argv){
	//check arguments
	if(argc < 2){
		printf("Missing Source\n");
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
	//do stuff
	//operation structure
	struct operation {
		//The parts we will manually add for each operation
		char *command;
		int Dest;
		int Source;
		char *Address; //address label
		//"Methods" will determine these
		int use_input_not;
		int write_enable;
		int Opcode;
		int Rd;
		int Rs;
		//the final output for each operation
		int Hex;
	} ops[MAXOPS]; //list of operations in order

	struct label {
		char name[16];
		int lineNumber;
	} labels[16];

	char code[100][20][20];

	int numLabels = 0;

	int counter = 0; //keep track of line #'s
	char line[100]; //temp store lines
	char *fun; //store functional token
	char *token; //store each word



	char funExp[] = "\n\t\r";
	char wordExp[] = " ,";
	char *eachWord[10];
	char **eachLine[MAXOPS];
	//while there are still lines
	while(fgets(line,sizeof line,fp)!= NULL){
		//get rid of garbage
		fun = strtok(line, funExp);
		while(fun != NULL){
			//printf("F%d: |%s|\n",counter, fun);
			//break lines into words
			int wordCount = 0;
			token = strtok(fun, wordExp);
			while(token != NULL){
				//printf("||%s|| ", token);
				//save each word in the line
				char *word = token;
				eachWord[wordCount] = word;
				int c;
				for(c=0; c<strlen(word); c++){
					code[counter][wordCount][c] = word[c];
					//printf("%c", code[counter][wordCount][c]);
				}
				//printf(" \n");
				printf("|%s| ", code[counter][wordCount]);

				/*
				if(word[strlen(word)-1] == ':'){
					strcp(labels[numLabels].name, word);
					labels[numLabels].lineNumber = counter;
				}*/

				wordCount++;
				token = strtok(NULL, wordExp);
			}
			//printf("\n");

			//go back through the each word in the line
			//printf("%2d: ", counter);
			int i;
			for(i=0; i<wordCount; i++){
				//printf("%s ", eachWord[i]);
			}
			printf("\n");
			fun = strtok(NULL,funExp);
		}
		eachLine[counter] = eachWord;
		//printf("L%d: %s\n", counter, line);
		counter++;
	}
	printf("\n");
	int i,j;
	for(i=0; i<counter; i++){
		printf("%d: ", i);
		j=0;
		char *sam = code[i][j];
		while(j<3 && sam[0] != NULL){
			if(sam[0] == ';') break;
			printf("|%s| ", sam);
			if(sam[strlen(sam)-1] == ':'){
				printf(" Label! ");
				int h = i;
				while(h<counter){
					if(isOp(code[h][0])){
						strcpy(labels[numLabels].name, sam);
						labels[numLabels].lineNumber = h;
						printf("label %d is %s", labels[numLabels].lineNumber, labels[numLabels].name);
						break;
					}
					h++;
				}
			}
			j++;
			sam = code[i][j];
		}
		printf("\n");
	}
	//next up:
	/*
		Enumerate labels
		Check for operation
		Methods for each opcode
		Output hex to printf & file
		Write instruction for use
		Edit Xilinx *bleh*
	*/


}