# -*- Makefile -*-

all: Calculator

Calculator: Main.o Get-Input.o Lex-Anal.o Fcts.o
	gcc -g -o Calculator Main.o Get-Input.o Lex-Anal.o Fcts.o -lm

Main.o: Main.c
	gcc -Wall -g -c Main.c -lm

Get-Input.o: Get-Input.c
	gcc -Wall -g -c Get-Input.c -lm

Lex-Anal.o: Lex-Anal.c
	gcc -Wall -g -c Lex-Anal.c -lm

Fcts.o: Fcts.c
	gcc -Wall -g -c Fcts.c -lm

clean :
	rm -f *.o Calculator -lm
