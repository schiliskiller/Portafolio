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
uint8_t assigned = 0;
Queue head;
char *wordTrack;

void
init(void) {
    system("clear||cls");
    printf(ABOUT);
    printf("( Presione Enter para empezar...");
    getchar();
}

void
exitProgram(void) {
    free(wordTrack);
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

void
dashWord(char **word) {
    char *dashtr;

    if (!(dashtr = malloc(strlen(*word)))) {
        fprintf(stderr, "ERROR: no space available\n");
        return;
    }

    strcpy(dashtr, "");

    for (char *i = *word;
         i < *word + strlen(*word);
         strcat(dashtr, "-"), i++)
        ;

    strcpy(*word, dashtr);
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
            tab.words = readLtrFile();

            for (Queue i = tab.words; head->pos <= M - 1; ) {
                uint8_t n = head->pos;

                if (locations[head->pos] == -1)
                    dashWord(&head->string);

                printf("%d - %s\n", n + 1, dequeue(&i, &head));
            }
            putchar('\n');
            pausa();
            break;
#endif
        case 0:
            puts("Hasta luego!");
            exitProgram();
            exit(EXIT_SUCCESS);
        default:
            printf("Elija otra opcion.\n\n");
            pausa();
            break;
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
        if (!((uint8_t) (i - tab + 1) % N)) putchar('\n');
    }
    putchar('\n');
}

bool
compareSol(char *word, tableData *tbl) {
    static uint8_t i = 0;
    
    if (*(word + i) == *(tbl->table + locations[head->pos] + i))
        tbl->table[i + locations[head->pos]] = '-';
    else
        return false;

    if (++i < strlen(word))
        compareSol(word, tbl);

    locations[head->pos] = -1;
    i = 0;

    return true;
}

bool
checkInput(tableData *tab, char *sol) {
    bool comp = true;

    tab->words = readLtrFile();
    
    while (head->pos < M - 1 && strcmp(sol, head->string))
        dequeue(&tab->words, &head);
    
    if (tab->words == NULL) comp = false;
    
    if (comp) comp = compareSol(sol, tab);
    
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

        if ((tbl[i-1] == letter || !((uint8_t) letter % 40)) && head->pos <= M - 1) {
            locations[head->pos] = i;

            char str[strlen(head->string) + 1];
            strcpy(str, dequeue(&tab->words, &head));

            assigned++;       
            
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
    uint8_t i = 1;

    if (!(wordTrack = malloc(128))) {
        fprintf(stderr, "ERROR: no space available\n");
        return NULL;
    }

    if (!(ltr = fopen("wordlist.ltr", "r"))) {
        fprintf(stderr, "Archivo no encontrado.\n");
        return NULL;
    }

    fscanf(ltr, " %[^\n]", wordTrack);
    wordTrack = realloc(wordTrack, strlen(wordTrack) + 1);

    enqueue(&wrds, &head, strtok(wordTrack, " "));
    wrds->pos = 0;
    while (wrds->pos < M - 1) {
        enqueue(&wrds, &head, strtok(NULL, " "));
        wrds->pos = i++;
    }

    fclose(ltr);

    return wrds;
}
