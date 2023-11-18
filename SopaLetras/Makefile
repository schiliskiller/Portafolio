CC=gcc
CFLAGS=-I.
LIBS=-lm
DEPS=letras_func.h data_struct_func.h
OBJ=letras_func.o data_struct_func.o SopaLetras.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

SopaLetras: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
