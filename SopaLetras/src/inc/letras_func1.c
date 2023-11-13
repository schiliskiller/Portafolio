#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "letras_func.h"

int locations[M] = {0};

void
menu(void) {
    puts("[1] Ingresar palabra");
#if DEBUG == 1
    puts("[2] Mostrar palabras");
#endif
    puts("[0] Salir\n");
    printf("$ ");
}

uint8_t
options(uint8_t op, tableData tab) {
    static uint8_t correct = 0;
    char sol[strmax(tab.words)];

    switch (op) {
        case 1:
            printf("Escriba las palabras: ");
            scanf(" %[^\n]", sol);

            if (checkInput(&tab, sol)) {
                if (++correct == M) {
                    system("clear||cls");
                    puts("-----SOPA DE LETRAS-----\n");
                    printTable(tab.table);
                    return 0;
                }
            } else {
                puts("Palabra no encontrada.\n");
                pausa();
            }
            break;
#if DEBUG == 1
        case 2:
            for (char **i = tab.words; i < tab.words + M; i++)
                printf("%lu - %s\n", i - tab.words + 1, *i);
            putchar('\n');
            pausa();
            break;
#endif
        case 0:
            puts("Hasta luego!");
            exit(EXIT_SUCCESS);
    }

    return 1;
}

void
pausa(void) {
    while (getchar() != '\n');
    printf("Presione Enter para continuar...");
    getchar();
}

void
printTable(char *tab) {
    for (char *i = tab; i < tab + N * N; i++) {
        printf("%c ", *i);
        if (!((int) (i - tab + 1) % N)) putchar('\n');
    }
    putchar('\n');
}

bool
checkInput(tableData *tab, char *sol) {
    bool comp = true;
    int i = 0;
    
    while (tab->wordpos < M && strcmp(sol, tab->words[tab->wordpos])) tab->wordpos++;
    
    if (tab->wordpos >= M) comp = false;
    
    if (comp) 
        while (i < strlen(tab->words[tab->wordpos])) {
            if (*(sol + i) == *(tab->table + locations[tab->wordpos] + i)) {
                tab->table[i + locations[tab->wordpos]] = '-';
                *((tab->words[tab->wordpos]) + i++) = '-';
            }
            else { comp = false; break; }
        }
    
    tab->wordpos = 0;

    return comp;
}

char
*tableGen(void) {
    srand((unsigned) time(NULL));

    char *tbl = malloc(N * N);
    char **wrds = readLtrFile();
    char letter;
    int j = 0;

    for (int i = 0; i < N * N; i++) {
        letter = rand() % 128;

        if (letter < 'A' || letter > 'Z') {i--; continue;}

        if ((tbl[i-1] == letter || !((int) letter % 40)) && j < M) {
            locations[j] = i;
            for (char *k = wrds[j]; k < wrds[j] + strlen(wrds[j]); k++)
                tbl[i++] = *k;
            i--; j++;
            continue;
        }

        if (letter >= 'A' && letter <= 'Z')
            tbl[i] = letter;
    }

    free(wrds);

    return tbl;
}

void
*threadTable(void *arg) {
    pthread_detach(pthread_self());

    tableData *tab = (tableData *) arg;
    tab->table = tableGen();
    tab->words = readLtrFile();
    tab->wordpos = 0;

    if (tab->table) pthread_exit((void *) (tab->table)); 
    else            pthread_exit(NULL);
}

int
strmax(char **wrds) {
    int n = 0;
    for (char **i = wrds; i < wrds + M; i++)
        if (strlen(*i) >= n)
            n = strlen(*i);
    
    return n;
}

char
**readLtrFile(void) {
    FILE *ltr;
    char **wrds = malloc(M * sizeof(char *));
    char *word = malloc(26);
    int i = 0;

    if (!(ltr = fopen("wordlist.ltr", "r"))) {
        fprintf(stderr, "Archivo no encontrado.\n");
        return NULL;
    }

    fscanf(ltr, " %[^\n]", word);

    wrds[i++] = strtok(word, " ");
    while (i < M)
        wrds[i++] = strtok(NULL, " ");

    fclose(ltr);

    return wrds;
}
