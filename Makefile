all: pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp

clean:
	rm -f *~ *.o

purge: clean
	-rm -f pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp


# Arquivos executaveis
pgmnega: pgmnega.o pgm.o
	gcc pgmnega.o pgm.o -o pgmnega

pgmrotacao: pgmrotacao.o pgm.o
	gcc pgmrotacao.o pgm.o -lm -o pgmrotacao

pgmlimiar: pgmlimiar.o pgm.o
	gcc pgmlimiar.o pgm.o -o pgmlimiar

pgmmedia: pgmmedia.o pgm.o
	gcc pgmmedia.o pgm.o -o pgmmedia

pgmmediana: pgmmediana.o pgm.o
	gcc pgmmediana.o pgm.o -o pgmmediana

pgmlbp: pgmlbp.o pgm.o
	gcc pgmlbp.o pgm.o -o pgmlbp

# Arquivos .o
pgm.o: pgm.c pgm.h
	gcc -c pgm.c -Wall

pgmnega.o: pgmnega.c pgm.h
	gcc -c pgmnega.c -Wall

pgmrotacao.o: pgmrotacao.c pgm.h
	gcc -c pgmrotacao.c -Wall

pgmlimiar.o: pgmlimiar.c pgm.h
	gcc -c pgmlimiar.c -Wall

pgmmedia.o: pgmmedia.c pgm.h
	gcc -c pgmmedia.c -Wall

pgmmediana.o: pgmmediana.c pgm.h
	gcc -c pgmmediana.c -Wall

pgmlbp.o: pgmlbp.c pgm.h
	gcc -c pgmlbp.c -Wall