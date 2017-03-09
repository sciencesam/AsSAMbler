.global main
main:

	start:
	CLR R16
	CLR R17
	LDI R18, 150
	loop:
	INC R18
	CP R16, R18
	BRMI loop  ;branch to loop if R16-R18 <0
	COM R17
	CLR R16
	JMP loop