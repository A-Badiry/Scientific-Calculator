# -*- Makefile -*-

all: Calculator

Calculator: Main.o Get-Input.o Lex-Anal.o Fcts.o
	gcc -g -o Calculator Main.o Get-Input.o Lex-Anal.o Fcts.o

Main.o: Main.c	lib.h
	gcc -Wall -g -c Main.c

Get-Input.o: Get-Input.c lib.h
	gcc -Wall -g -c Get-Input.c

Lex-Anal.o: Lex-Anal.c lib.h
	gcc -Wall -g -c Lex-Anal.c

Fcts.o: Fcts.c lib.h
	gcc -Wall -g -c Fcts.c

clean :
	rm -f *.o Calculator
