//Sam Stone EECE@Binghamton 3/8/2017

/*
This is a simple assembler for EECE@Binghamton. Compile with MAKEFILE.
To Build:
	make build

To Run: 
	asm <input file> <optional output file>

Here is a Table of commands:

*** TODO: list of commands ***

*/

/* TODO
	
	add data to operations
		opcode, hex
	output full hex to file
	write instructions
	Maybe edit Xilinx
*/

#include <stdio.h> //printf //
#include <stdlib.h> //fprint, fgets //
#include <string.h> //strcmp, strcpy, strtok, strcat //
#include <math.h>  //log //

int isReg(char *s);
int isOpcode(char *s);
int isLabel(char *s);
int isComment(char *s);

int addToRegisterList(char *s);
int addToOpcodeList(char *s);
int fufillLabels(int opCount, int labelCount);

int registerNameToNumber(char *s);
int enumerateDataInOperations();
 

#define MAXOPS 34
char opcodes[MAXOPS][5] = {
	"MOV","EOR","AND","OR","ADD",
	"SUB","LSR","MUL","JMP","BCC","BCS",
	"BZC","BZS","BNC","BNS","CP",
	"LDI","EOR","AND","OR","ADD",
	"SUB","LSR","MUL","JMP","BCC",
	"BCS","BZC","BZS","BNC","BNS",
	"CPI", "CLR", "NOP"
};
//define datatypes//
	struct {
		char name[20];
		int line;
	} labels[100];
	int nLabels = 0;

	struct {
		char name[20];
		int location;
	} registers[100];
	int nReg = 0;

	struct {
		char name[20];
		char label[20];
		char Dest[20];
		char Src[20];

		int ui; //use input//
		int wen; //write enable//

		int opcode;
		// Rd and Rs// 
		int Rd;
		int Rs;
		//label address for branch//
		int Addr;

		char hex[20]; //final output//

	} operations[100];
	int nOps = 0;

int main(int argc, char **argv){
	//check arguments//
	if(argc < 2){
		printf("Missing Source File\n");
		return 1;
	}
	//open input and output files//
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
	

	typedef char * string;
	//collect line by line and format//
	char funExp[] = "\n\n\t\r ,"; //delimiters//
	char *token; //token for words//
	char line[100]; //temp storage for each line//
	char operation[3][20]; //temp operation storage//
	int cCount = 2;
	while(fgets(line, sizeof line, fp)!=NULL){
		token = strtok(line, funExp);
		cCount = 2;
		while(token != NULL && cCount >= 0 ){
			//printf("|%s| ", token);
			//Copy each operation to list//
			strcpy(operation[cCount], token);
			cCount--;
			token = strtok(NULL, funExp);
		}
		//printf("\n");
		//Characterize Input//
		if(isLabel(operation[2])){
			//printf("%s is a Label\n", operation[2]);
			//add to label database//
			strcpy(labels[nLabels].name, operation[2]);
			labels[nLabels].line = -1;
			nLabels++;
		}
		if(isOpcode(operation[2])){
			fufillLabels(nOps, nLabels);
			//printf("%s is an opcode\n", operation[2]);
			strcpy(operations[nOps].name , operation[2]);
			//Initizlize Rd, Rs, and Addr to -1 for later logic//
			operations[nOps].Rd = -1;
			operations[nOps].Rs = -1;
			operations[nOps].Addr = -1;
			//printf("O:%s\n", operations[nOps].name);
			if(isReg(operation[1])){
				//printf("%s is Rd\n", operation[1]);
				addToRegisterList(operation[1]);
				operations[nOps].Rd = registerNameToNumber(operation[1]);
				strcpy(operations[nOps].Dest, operation[1]);
				if(isReg(operation[0])){
					//printf("%s is Rs\n", operation[0]);
					addToRegisterList(operation[0]);
					operations[nOps].Rs = registerNameToNumber(operation[0]);
					strcpy(operations[nOps].Src, operation[0]);
					operations[nOps].ui = 0;
					if(strcmp(operation[2], "CP" ) == 0 || strcmp(operation[2], "CPI" ) == 0){
						operations[nOps].wen = 1;
					}
				}
				else if(!(strcmp(operation[2], "CLR") ==0)) {
					//printf("%s uses Input\n", operation[2]);
					operations[nOps].ui = 1;
				}
				else {
					//printf("%s only modifies %s\n", operation[2], operation[1]);
					operations[nOps].wen = 0;
				}
			}
			else {
				//printf("%s is a branch to %s\n", operation[2], operation[1]);
				strcpy(operations[nOps].label, operation[1]);
				strcat(operations[nOps].label, ":");
				operations[nOps].wen = 1;
			}
			nOps++;
		}
		//int lastOp = nOps-1;
		//if(lastOp >= 0){
			//printf("\n\t%d: %s %s %s %s\n", lastOp,operations[lastOp].name,
			 //operations[lastOp].Dest, operations[lastOp].Src, operations[lastOp].label);
		//}
		
		//printf("\n");

	} //end of input//

	enumerateDataInOperations();

	int i;
	for(i=0;i<nOps;i++){
		/*printf("%d: OP:%s D:%s S:%s L:%s | UI:%d wen:%d Rd:%d Rs:%d Addr:%d Opc:%d 0x:%s \n\n", i,operations[i].name,
			operations[i].Dest, operations[i].Src, operations[i].label, operations[i].ui, operations[i].wen, operations[i].Rd,
			 operations[i].Rs, operations[i].Addr,operations[i].opcode, operations[i].hex);*/
		printf("%s\n", operations[i].hex);
		fprintf(ofp, "%s\n", operations[i].hex);
	}
	return 0;
}

int isReg(char *s){
	//All registers are labeled with an 'R'//
	if(toupper(s[0]) == 'R') return 1;
	else return 0;
}
int isOpcode(char *s){
	//look through opcode dictionary for a match//
	int i;
	for(i=0; i<MAXOPS; i++){
		if(strcmp(s, opcodes[i]) == 0) return 1;
	}
	return 0;
}
int isLabel(char *s){
	//The last character of a label is ':'//
	if(s[strlen(s)-1] == ':') return 1;
	else return 0;
}
int addToRegisterList(char *s){
	int i;
	for(i=0; i<nReg; i++){
		if(strcmp(registers[i].name, s) == 0){
			return 0;
		}
	}
	strcpy(registers[nReg].name, s);
	registers[nReg].location = nReg;
	nReg++;
	return 1;
}
int fufillLabels(int opCount, int labelCount){
	//for each label, if its destination is empty and its name isn't//
	//the destination is opCount, increment local label count//
	int llc = 0;
	int i;
	for(i=0; i<=nLabels; i++){
		if(labels[i].name != NULL && labels[i].line == -1){
			labels[i].line = opCount;
			//printf("Label %s is at line %d\n", labels[i].name, labels[i].line);
			llc++;
		}
	}
	return llc;
}
int isComment(char *s){
	// Comments start with a semicolon//
	if(s[0] == ';') return 1;
	else return 0;
}
int registerNameToNumber(char *s){
	int i;
	for(i=0; i<nReg; i++){
		if(strcmp(registers[i].name, s) == 0){
			return registers[i].location;
		}
	}
	return -1;
}
int enumerateLabels(){
	int i;
	for(i=0; i<nOps; i++){
		if(operations[i].label[0] != 0){
			//printf("Labeling %s\n", operations[i].label);
			int j;
			for(j=0; j<nLabels; j++){
				//printf("%s\n", labels[j].name);
				if(strcmp(operations[i].label, labels[j].name) == 0){
					//printf("%s and %s are a match\n", operations[i].label, labels[j].name);
					operations[i].Addr = labels[j].line;
					break;
				}
			}
		}
	}

}
int generateOpcodes(){
	int i;
	for(i=0; i<nOps; i++){
		int j;
		for(j=0; j<MAXOPS; j++){
			if(strcmp(opcodes[j], operations[i].name)==0){
				operations[i].opcode = j;
				if((j<7) && 
				 	!(strcmp(operations[i].name, "LDI") == 0) && operations[i].ui == 1){
					//the inputs that use input are 16 higher in the sequence//
					operations[i].opcode += 16;
				}
				break; //if we found it//
			}
		}
		//couple special conditions for custom commands//
		if(strcmp(operations[i].name, "CLR") ==0){
			//CLR Rd = EOR Rd Rd//
			operations[i].Rs = operations[i].Rd;
			operations[i].opcode = 1; //EOR is opcode 1
		} 
		if(strcmp(operations[i].name, "NOP") ==0){
			//NOP = MOV Rd Rd//
			operations[i].Rs = operations[i].Rd;
			operations[i].opcode = 0; //MOV is opcode 0
		} 
	}
	return 1;
}
char* toHex(int numberB10, int bits){
	//returns the base 10 number in the base 16//
	const float log16 = 1.204;
	//const int numChars = ceil((float) log(numberB10?numberB10:numberB10+1)/log(16))+1;
	const int numChars = bits;
	static char theWord[3]={0x00};
	static char *hex;
	hex = malloc(sizeof(char)*(numChars));
	//printf("Numchars = %d Number= %d\n", numChars, numberB10);
	int j;
	int theNumber;
	for(j = numChars-1; j>=0; j--){
		//printf("r\n");
		theNumber = numberB10%16;
		if(theNumber <=9) hex[j] = '0'+theNumber;
		if(theNumber >9) hex[j] = 'A'+theNumber-10;
		numberB10 = numberB10/16;  
	}
	//if(theWord[0] == '0') theWord[0] = ' ';
	return hex;
}
int formatHex(){
	char stemp[10];
	int temp;
	int i;
	for(i=0; i<nOps; i++){
		//printf("%s\n", toHex(operations[i].opcode));
		strcpy(operations[i].hex, toHex(operations[i].opcode, 2));
		//printf("s\n");
		if(operations[i].Addr != -1){
			strcat(operations[i].hex, toHex(operations[i].Addr, 1));
		}
		else if(operations[i].Rs != -1){
			temp = operations[i].Rd*4 + operations[i].Rs;
			strcat(operations[i].hex, toHex(temp, 1));
		}
		else {
			temp = operations[i].Rd*4;
			strcat(operations[i].hex, toHex(temp, 1));
		}
	}
	return 0;
}
int enumerateDataInOperations(){
	enumerateLabels();
	generateOpcodes();
	formatHex();
	return 0;
}