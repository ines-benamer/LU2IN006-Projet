# OULD BOUALI Louisa
# BENAMER BELKACEM Inès

all : Chaine.o SVGwriter.o Reseau.o main.o 
	gcc Chaine.o SVGwriter.o main.o -o main -lm

Chaine.o : Chaine.c Chaine.h
	gcc -Wall -c Chaine.c -o Chaine.o -lm

Reseau.o : Chaine.c Chaine.h
	gcc -Wall -c Reseau.c -o Reseau.o

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc -Wall -c SVGwriter.c -o SVGwriter.o

main.o : ChaineMain.c SVGwriter.h Chaine.h
	gcc -Wall -c ChaineMain.c -o main.o




clean :
	rm *.o
	rm main