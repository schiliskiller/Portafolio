#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_struct_func.h"

/* Stack */
void
pop(DirectStack stck) {
    Stack prev = (*stck)->next;

    free(*stck);
    *stck = prev;
}

void
push(DirectStack stck, char * value) {
    Stack newNode;

    CREATE_NODE(newNode, sizeof(struct node));

    newNode->string = value;
    newNode->next = *stck;

    *stck = newNode;
}

void
printStack(Stack strct) {
    for (Stack i = strct; i != NULL; i = i->next)
        puts(i->string);
    fputc('\n', stdout);
}

/* Queue */

void
enqueue(DirectQueue newQueue, DirectQueue headNode, char * value) {
    Queue newTail;

    CREATE_NODE(newTail, sizeof(struct node));

    newTail->string = value;
    newTail->next = *newQueue;
    *headNode = !*newQueue ? newTail : *headNode;

    *newQueue = newTail;
}

void
enque(DirectQueue queue, DirectQueue headNode, char * value1, char * value2) {
    Queue newTail, newHead;

    CREATE_NODE(newTail, sizeof(struct node));
    CREATE_NODE(newHead, sizeof(struct node));

    newTail->string = value1;
    newHead->string = value2;

    newTail->next = *queue;
    newHead->next = (*headNode)->next;
    (*headNode)->next = newHead;

    *headNode = newHead;
    *queue = newTail;
}

void
deque(DirectQueue queue, DirectQueue headNode) {
    Queue prevNode, nextNode = *queue;

    // Deleting tail
    *queue = (*queue)->next;
    free(nextNode);

    // Deleting head
    for (prevNode = *queue;
         prevNode->next != *headNode;
         prevNode = prevNode->next)
        ;

    free(*headNode);
    *headNode = prevNode;
    (*headNode)->next = NULL;
}

char
*dequeue(DirectQueue queue, DirectQueue headNode) {
    Queue prevNode;
    char *str = malloc(128);

    for (prevNode = *queue;
         prevNode->next != *headNode && prevNode->next != NULL;
         prevNode = prevNode->next)
        ;

    strcpy(str, (*headNode)->string);
    str = realloc(str, strlen(str));

    free(*headNode);
    *headNode = prevNode;
    if (*headNode)
        (*headNode)->next = NULL;

    return str;
}

void
printQueue(Queue queue) {
    for (Queue i = queue; i != NULL; i = i->next)
        puts(i->string);
    fputc('\n', stdout);
}

/* Linked Lists */
void
insertNode(DirectList list, char * n) {
    List newNode;

    CREATE_NODE(newNode, sizeof(struct node));

    newNode->string = n;
    newNode->next = *list;

    *list = newNode;
}

void
printList(List p) {
    while (p) {
        puts(p->string);
        p = p->next;
    }
    putchar('\n');
}

void
deleteNode(List list, char * n) {
    List p;
    for (p = list;
         p && p->string != n;
         list = p, p = p->next)
        ;

    if (!p) return;
    else if (!list) list = list->next;
    else list->next = p->next;
    
    free(p);
}

List
findNode(List list, char * n) {
    while (list->string != n)
        list = list->next;

    return list;
}

/* Testing */
void
fbbf(DirectList list) { /* Final-Begin; Begin-Final*/
    List p = *list, q = NULL;

    for (; p != NULL; q = p, p = p->next)
        ;

    p = (*list)->next;
    q->next = p;

    while (p->next != q) p = p->next;

    p->next = *list;
    (*list)->next = NULL;
    *list = q;
}

void
fb(DirectList list) { /* Final-Begin*/
    List p = *list, q = NULL;

    for (; p != NULL; q = p, p = p->next)
        ;

    p = *list;
    q->next = p;

    while (p->next != q) p = p->next;

    p->next = NULL;
    *list = q;
}

/* void /1* Begin-Final *1/ */
/* bf(DirectList list) { */
    
/* } */

/* void */
/* readSent(struct letters **list, char *sent) { */
/*     for (char *c = sent + strlen(sent) - 1; */
/*          c >= sent; */
/*          insertLetter(list, *c--)) */
/*         ; */
/* } */

/* void */
/* replaceLetter(struct letters *list, char c1, const char c2) { */
/*     struct letters *p, *newChar; */

/*     if (!(newChar = malloc(sizeof(struct letters)))) { */
/*         printf("ERROR: malloc cannot allocate more space\n"); */
/*         exit(EXIT_FAILURE); */
/*     } */


/*     for (p = list; */
/*          p && p->letter != c1; */
/*          list = p, p = p->next) */
/*         ; */

/*     if (!p) return; */

/*     list->next = newChar; */
/*     newChar->next = p->next; */
    
/*     free(p); */
/* } */
