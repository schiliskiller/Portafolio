#ifndef DATA_STRUCT
#define DATA_STRUCT

#define CREATE_NODE(x, y) if (!((x) = malloc((y)))) {                         \
                              fprintf(stderr, "Error: no space available\n"); \
                              return;                                         \
                          }

#define ABOUT "/-----------------------------\\\n" \
              "| Sopa de letras:             |\n" \
              "|                             |\n" \
              "| Martinez Schleske Alan      |\n" \
              "| Ingenieria en Informatica   |\n" \
              "| Universidad Veracruzana (c) |\n" \
              "|                             |\n" \
              "| (.2023)                     |\n" \
              "\\-----------------------------/\n" \
              "  /                            \n" \
              " /                             \n" \

/* In general terms... */
struct node {
    char *string;
    unsigned int pos;
    struct node *next;
};

/* Stack */
typedef struct node* Stack;
typedef struct node** DirectStack;

void push(DirectStack stck, char * value);
void pop(DirectStack stck);
void printStack(Stack strct);

/* Queue */
typedef struct node* Queue;
typedef struct node** DirectQueue;

void enqueue(DirectQueue queue, DirectQueue head, char * value);
char *dequeue(DirectQueue queue, DirectQueue headNode);
void enque(DirectQueue queue, DirectQueue headNode, char * value1, char * value2);
void deque(DirectQueue queue, DirectQueue headNode);
void printQueue(Queue queue);

/* Linked Lists */
typedef struct node* List;
typedef struct node** DirectList;

void insertNode(DirectList list, char * n);
void printList(List p);
void deleteNode(List list, char * n);
List findNode(List list, char * n);

/* Testing */
void fbbf(DirectList list);
void fb(DirectList list);
/* void bf(DirectList list); */

/* void readSent(DirectList list, char *sent); */
/* void replaceLetter(List list, char c1, const char c2); */

#endif
