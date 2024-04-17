# OULD BOUALI Louisa
# BENAMER BELKACEM Inès

CC = gcc
CFLAGS = -Wall

all : main main_chaines main_reseau

main : Chaine.o SVGwriter.o main_chaines.o
	$(CC) Chaine.o SVGwriter.o main_chaines.o -o main -lm

main_chaines : main_chaines.o SVGwriter.o Chaine.o
	$(CC) main_chaines.o SVGwriter.o Chaine.o -o main_chaines -lm

main_reseau : main_reseau.o SVGwriter.o Reseau.o Chaine.o
	$(CC) main_reseau.o SVGwriter.o Reseau.o Chaine.o -o main_reseau -lm


Chaine.o : Chaine.c Chaine.h
	$(CC) $(CFLAGS) -c Chaine.c -o Chaine.o 

Reseau.o : Reseau.c Reseau.h Chaine.h
	$(CC) $(CFLAGS) -c Reseau.c -o Reseau.o 

Hachage.o : Hachage.c Hachage.h Reseau.h
	$(CC) $(CFLAGS) -c Hachage.c -o Hachage.o

ArbreQuat.o : ArbreQuat.c ArbreQuat.h Chaine.h Reseau.h
	$(CC) $(CFLAGS) -c ArbreQuat.c -o ArbreQuat.o

SVGwriter.o : SVGwriter.c SVGwriter.h
	$(CC) $(CFLAGS) -c SVGwriter.c -o SVGwriter.o 

main_chaines.o : main_chaines.c SVGwriter.h Chaine.h
	$(CC) $(CFLAGS) -c main_chaines.c -o main_chaines.o 

main_reseau.o : main_reseau.c SVGwriter.h Reseau.h Chaine.h
	$(CC) $(CFLAGS) -c main_reseau.c -o main_reseau.o


.PHONY : clean
clean :
	rm -f *.o main main_chaines main_reseau
