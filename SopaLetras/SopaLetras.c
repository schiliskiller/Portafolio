#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "letras_func.h"
#include "data_struct_func.h"

int
main(void) {
    pthread_t id;

    tableData tab;
    uint8_t status, op;

    pthread_create(&id, NULL, threadTable, &tab);

    sleep(1);

    do {
        system("clear||cls");
        puts("-----SOPA DE LETRAS-----\n");
        printTable(tab.table);

        menu();
        scanf("%" SCNu8, &op);
        status = options(op, tab);
    } while (status);

    puts("Gracias por jugar");

    free(tab.table);
    
    return 0;
}
