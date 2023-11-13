CC=gcc
CFLAGS=-I.
LIBS=-lm
LOC = ~/Documentos/codigo/c/SopaLetras
DEPS=$(LOC)/src/inc/letras_func.h $(LOC)/src/inc/data_struct_func.h
OBJ=$(LOC)/src/inc/letras_func.o $(LOC)/src/inc/data_struct_func.o SopaLetras.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

SopaLetras: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
