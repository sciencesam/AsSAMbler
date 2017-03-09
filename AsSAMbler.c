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

	char codes[100][15][20] = NULL; //100 lines, 15 words per line, 20 chars per line

	int wordCount = 0;
	int lineCount = 0; //keep track of line #'s
	char line[100]; //temp store lines
	char *fun; //store functional token
	char *token; //store each word

	char funExp[] = "\n\t\r";
	char wordExp[] = " ,";
	//while there are still lines
	while(fgets(line,sizeof line,fp)!= NULL){
		//get rid of garbage
		fun = strtok(line, funExp);
		while(fun != NULL){
			//break lines into words
			wordCount = 0;
			token = strtok(fun, wordExp);
			while(token != NULL){
				//save each word in the line
				//char *word = token;

				wordCount++;
				token = strtok(NULL, wordExp);
			}
			fun = strtok(NULL,funExp);
		}
		lineCount++;
	}

	//next up:
	/*
		Import each line
		Check for labels
			Enumerate labels
		Check for operation
		Methods for each opcode
		Output hex to printf & file
		Write instruction for use
		Edit Xilinx *bleh*
	*/


}