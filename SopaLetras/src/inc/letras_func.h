#ifndef LETRAS_FUNC
#define LETRAS_FUNC

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "data_struct_func.h"

#define DEBUG 1
#define M 6
#define N 12

struct tableData {
    char *table;
    Queue words;
};

typedef struct tableData tableData;

void __attribute__ ((constructor)) init(void);
void menu(void);
uint8_t options(uint8_t op, tableData tab);
void pausa(void);
char *tableGen(tableData *tab);
void *threadTable(void *arg);
void printTable(char *tab);
/* int strmax(void); */
Queue readLtrFile(void);
bool checkInput(tableData *tab, char *sol);

#endif
