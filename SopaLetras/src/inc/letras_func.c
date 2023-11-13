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
#include "data_struct_func.h"

int locations[M] = {0};
Queue head;
unsigned int assigned = 0;

void
init(void) {
    printf(ABOUT);
    printf("( Presione Enter para empezar...");
    getchar();
}

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
    char sol[51];

    switch (op) {
        case 1:
            printf("Escriba las palabras: ");
            scanf(" %[^\n]", sol);

            if (checkInput(&tab, sol)) {
                if (++correct == assigned) {
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

    tab->words = readLtrFile();
    
    while (head->pos < M && strcmp(sol, head->string))
        dequeue(&tab->words, &head);
    
    if (tab->words == NULL) comp = false;
    
    if (comp) {
        int i = 0;

        while (i < strlen(head->string)) {
            if (*(sol + i) == *(tab->table + locations[head->pos] + i)) {
                tab->table[i + locations[head->pos]] = '-';
                *(head->string + i++) = '-';
            }
            else { comp = false; break; }
        }
    } 
    
    free(tab->words);

    return comp;
}

char
*tableGen(tableData *tab) {
    char *tbl;
    char letter;

    if (!(tbl = malloc(N * N))) {
        fprintf(stderr, "ERROR: no space available\n");
        return NULL;
    }

    srand((unsigned) time(NULL));

    for (int i = 0; i < N * N; i++) {
        letter = rand() % 128;

        if (letter < 'A' || letter > 'Z') {i--; continue;}

        if ((tbl[i-1] == letter || !((int) letter % 40)) && head->pos <= M - 1) {
            locations[head->pos] = i;
            assigned++;       
            char str[50];
            strcpy(str, dequeue(&tab->words, &head));
            
            for (char *k = str; k < str + strlen(str);  k++)
                tbl[i++] = *k;
            i--;
            continue;
        }

        if (letter >= 'A' && letter <= 'Z')
            tbl[i] = letter;
    }

    return tbl;
}

void
*threadTable(void *arg) {
    pthread_detach(pthread_self());

    tableData *tab = (tableData *) arg;
    tab->words = readLtrFile();
    tab->table = tableGen(tab);

    pthread_exit(NULL);
}

/* int */
/* strmax(void) { */
/*     Queue wrds = readLtrFile(); */
/*     int n = 0; */

/*     while (wrds != NULL) { */
/*         if (strlen(wrds->string) >= n) */
/*             n = strlen(wrds->string); */
/*         dequeue(&wrds, &head); */
/*     } */

/*     free(wrds); */
    
/*     return n; */
/* } */

Queue
readLtrFile(void) {
    FILE *ltr;
    Queue wrds = NULL;
    char *word;
    unsigned int i = 0;

    if (!(word = malloc(128))) {
        fprintf(stderr, "ERROR: no space available\n");
        return NULL;
    }

    if (!(ltr = fopen("wordlist.ltr", "r"))) {
        fprintf(stderr, "Archivo no encontrado.\n");
        return NULL;
    }

    fscanf(ltr, " %[^\n]", word);
    word = realloc(word, strlen(word) + 1);

    /* wrds[i++] = strtok(NULL, " "); */
    enqueue(&wrds, &head, strtok(word, " "));
    wrds->pos = i++;
    while (wrds->pos < M - 1) {
        enqueue(&wrds, &head, strtok(NULL, " "));
        wrds->pos = i++;
    }
        /* wrds[i++] = strtok(NULL, " "); */

    fclose(ltr);

    return wrds;
}
