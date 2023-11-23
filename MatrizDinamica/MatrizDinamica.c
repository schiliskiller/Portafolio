#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h> /* Para uso de la funcion "pause()" si el usuario usa Windows */
#include <inttypes.h>
#include <math.h>

#define NUM_OP 4

/* Tipo booleando */
typedef enum {FALSE, TRUE} Bool;

/* Tipo-estructura para una matriz */
typedef struct {
    int16_t  **matriz;
    uint16_t m, n;
} Matriz;

/* Funciones para el manejo de matrices */
Bool crearMatriz(Matriz *, uint16_t m, uint16_t n);
Bool redimMatriz(Matriz *, uint16_t m, uint16_t n);
Bool llenarMatriz(Matriz *, int16_t *datos);
void mostrarMatriz(Matriz *mat, uint16_t m, uint16_t n);
void liberarMatriz(Matriz *mat);
uint8_t numDigMax(Matriz *mat);

/* Opciones del menu */
void op1(Matriz *);
void op2(Matriz *);
void op3(Matriz *);
void op4(Matriz *);

/* Si el sistema NO contiene el nucleo Windows NT: */
/* Declarar funcion para sistemas Unix */
#ifndef _WIN32
void pauseUnix(void);
#endif

uint16_t pos;

/*-------------------- Programa --------------------*/
int
main(void) {
    /* Declaracion de variable con tipo Matriz (vease de la linea 9 - 14) */
    Matriz mat = {NULL, 0, 0};
    /* Arreglo de punteros a funciones */
    void (*opciones[NUM_OP])(Matriz *) = {op1, op2, op3, op4};

    uint8_t op;

    do {
        system("clear||cls");
        if (!mat.matriz)
            printf("Sin matriz\n"
                   "------------------------\n");
        else
            printf("Matriz %" PRIu16 "x%" PRIu16 "\n"
                   "------------------------\n",
                   mat.m, mat.n);
        printf("Elija la opcion:\n\n"
               "[1] Crear matriz\n"
               "[2] Llenar matriz\n"
               "[3] Redimensionar matriz\n"
               "[4] Mostrar matriz\n"
               "[0] Salir\n\n"
               "$ ");
        scanf(" %" SCNu8, &op);
        
        if (op > 0 && op <= NUM_OP) {
            (*opciones[op - 1])(&mat);
#ifdef _WIN32           /* (vease linea 32 - 36) */
            pause();
#else
            pauseUnix();
#endif
        }
    } while (op);

    printf("\nHasta luego!\n");

    liberarMatriz(&mat);
    
    return 0;
}

/*---------------------- Funciones ----------------------*/
void
pauseUnix(void) {
    while (getchar() != '\n');
    printf("\nPresione Enter para continuar...");
    getchar();
}

/*---------------------- Opciones del menu ----------------------*/
void
op1(Matriz *mat) {
    uint16_t i, j;
    printf("Dimensiones de la matriz? (mxn) [0x0 para cancelar]: ");
    scanf("%" SCNu16 "%*c%" SCNu16, &i, &j);

    if (!i && !j) {
        printf("\nProceso cancelado\n");
        return;
    }

    if (crearMatriz(mat, i, j))
        printf("\nMatriz %" PRIu16 "x%" PRIu16 " creada!\n",
                mat->m, mat->n);
}

void
op2(Matriz *mat) {
    int16_t *entrada;

    if (!mat->matriz) {
        printf("\nMatriz inexistente o no creada\n");
        return;
    }

    if (!(entrada = malloc(mat->m * mat->n * sizeof(*entrada)))) {
        printf("ERROR: espacio no disponible o memoria llena\n");
        return;
    }

    printf("Ingrese %" PRIu16 " datos: ", mat->m * mat->n);
    for (uint16_t i = 0; i < mat->m * mat->n; i++) 
        scanf(" %" SCNi16, entrada + i);

    if (llenarMatriz(mat, entrada))
        printf("\nMatriz llenada!\n");

    free(entrada);
}

void
op3(Matriz *mat) {
    uint16_t i, j;

    if (!mat->matriz) {
        printf("\nMatriz inexistente o no creada\n");
        return;
    }
    printf("Nuevas dimensiones de la matriz? (mxn) [0x0 para cancelar]: ");
    scanf("%" SCNu16 "%*c%" SCNu16, &i, &j);

    if (!i && !j) {
        printf("\nProceso cancelado\n");
        return;
    }

    if (redimMatriz(mat, i, j))
        printf("\nMatriz redimensionada a %" PRIu16 "x%" PRIu16 "!\n",
                mat->m, mat->n);
}

void
op4(Matriz *mat) {
    if (mat->matriz && mat->matriz[0][0]) {
        mostrarMatriz(mat, mat->m, mat->n);
        return;
    }

    printf("\nMatriz inexistente o no llenada\n");
}

/*------------------- Funciones para el manejo de matrices -------------------*/
void
liberarMatriz(Matriz *mat) {
    for (uint16_t i = 0; i < mat->m; i++)
        free(mat->matriz[i]);
    free(mat->matriz);
}

Bool
crearMatriz(Matriz *mat, uint16_t m, uint16_t n) {
    if (mat->matriz) liberarMatriz(mat);

    if (!(mat->matriz = malloc(m * sizeof(*mat->matriz)))) {
        printf("ERROR: espacio no disponible\n");
        return FALSE;
    }

    for (uint16_t i = 0; i < m; i++)
        *(mat->matriz + i) = calloc(n, sizeof(**mat->matriz));

    pos = m - 1;
    mat->m = m;
    mat->n = n;
    
    return TRUE;
}

Bool
redimMatriz(Matriz *mat, uint16_t m, uint16_t n) {
    if (m > 0 && mat->m != m) {
        static int16_t *p = NULL;

        if (m < mat->m) {
            p = mat->matriz[pos];
        } else {
            if (!(mat->matriz = realloc(mat->matriz, m * sizeof(*mat->matriz)))) {
                printf("ERROR: no es posible ocupar mas espacio\n");   
                return FALSE;
            }

            for (uint16_t i = pos; i > mat->m; i--, p -= 0x20)
                mat->matriz[i] = p;
            mat->m = pos + 1;
        }

        /* NOTA:
         *
         * La constante hexadecimal '0x20' es el resultado de la diferencia entre
         * la direccion de un puntero doble y otro, el cual permite ubicar bien la
         * direccion correcta decrementandole al puntero que guarda la ultima direccion
         * la constante hexa. Esta analisis se llevo a cabo con un debugger (gdb) y con
         * el uso de operaciones aritmeticas en hexadecimal.
         * */ 


        for (uint16_t i = mat->m; i < m; i++)
            if (!(*(mat->matriz + i) = calloc(mat->n, sizeof(**mat->matriz)))) {
                printf("ERROR: no es posible ocupar mas espacio\n");   
                return FALSE;
            }

        mat->m = m;
    }

    if (n > 0 && mat->n != n) {
        for (uint16_t i = 0; i < mat->m; i++)
            if (!(*(mat->matriz + i) = realloc(*(mat->matriz + i), n * sizeof(**mat->matriz)))) {
                printf("ERROR: no es posible ocupar mas espacio\n");   
                return FALSE;
            }

        mat->n = n;
    }

    return TRUE;
}

void
mostrarMatriz(Matriz *mat, uint16_t m, uint16_t n) {
    if (!m) return;

    mostrarMatriz(mat, m - 1, n);

    const uint16_t max_dig = numDigMax(mat);

    for (uint16_t i = 0; i < n; i++)
        switch (max_dig) {
            case 1:
                printf(" %2" PRIi16, mat->matriz[m - 1][i]);
                break;
            case 2:
                printf(" %3" PRIi16, mat->matriz[m - 1][i]);
                break;
            case 3:
                printf(" %4" PRIi16, mat->matriz[m - 1][i]);
                break;
            case 4:
                printf(" %5" PRIi16, mat->matriz[m - 1][i]);
                break;
            case 5:
                printf(" %6" PRIi16, mat->matriz[m - 1][i]);
                break;
        }
    putchar('\n');

}

uint8_t
numDigMax(Matriz *mat) {
    uint8_t max = 0;

    for (int i = 0; i < mat->m; i++)
        for (int j = 0; j < mat->n; j++)
            if (mat->matriz[i][j] >= max)
                max = log10(mat->matriz[i][j]) + 1;

    return max;
}

Bool
llenarMatriz(Matriz *mat, int16_t *datos) {
    int k = 0;

    if (!mat) return FALSE;

    for (int i = 0; i < mat->m; i++)
        for (int j = 0; j < mat->n; j++)
            *(*(mat->matriz + i) + j) = *(datos + k++);   


    return TRUE;
}

