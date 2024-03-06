# OULD BOUALI Louisa
# BENAMER BELKACEM Inès

all : Chaine.o SVGwriter.o main.o 
	gcc Chaine.o SVGwriter.o main.o -o main

Chaine.o : Chaine.c Chaine.h
	gcc -Wall -c Chaine.c -o Chaine.o -lm

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc -Wall -c SVGwriter.c -o SVGwriter.o

main.o : ChaineMain.c SVGwriter.h Chaine.h
	gcc -Wall -c ChaineMain.c -o main.o -lm


clean :
	rm *.o
	rm main