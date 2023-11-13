#ifndef LETRAS_FUNC
#define LETRAS_FUNC

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define DEBUG 0
#define M 6
#define N 12

typedef struct {
    char *table;
    char **words;
    int wordpos;
} tableData;

void init(void);
void menu(void);
uint8_t options(uint8_t op, tableData tab);
void pausa(void);
char *tableGen(void);
void *threadTable(void *arg);
void printTable(char *tab);
int strmax(char **wrds);
char **readLtrFile(void);
bool checkInput(tableData *tab, char *sol);

#endif
