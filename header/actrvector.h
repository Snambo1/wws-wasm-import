#ifndef ACTRVECTOR_H
#define ACTRVECTOR_H
#include "actralloc.h"

extern void log_word(char *thing, int length);
extern void log_address(void *thing);
extern void log_int(int val);
struct ActrVector
{
    int pointer;
    int length;
    void *head;
};

void actr_vector_add(struct ActrVector *list, int item)
{
    void *head;
    //log_word("actr_vector_add entry");
    //log_address(list->head);
    if (list->length == 0)
    {
        list->head = actr_malloc(sizeof(void *) * 8);
        //log_word("actr_vector_add first");
        //log_address(list->head);
        list->length = 8;
    }
    else if (list->pointer >= list->length)
    {
        // log_word("actr_vector_add resize", 22);

        void *newHead = actr_malloc(sizeof(void *) * (list->length + 8));
        head = list->head;
        list->head = newHead;
        list->length += 8;
        for (int i = 0; i < list->pointer; i++)
        {
            *(int *)newHead = *(int *)head;
            newHead += sizeof(void *);
            head += sizeof(void *);
        }
        actr_free(list->head);
    }
    //log_word("last");
    head = list->head + list->pointer * sizeof(void *);
    // log_word((int*)item, strlen((char*)item));
    //*(int*)head = *(int*)item;
    *(int*)head = (int)item;
    // test = &item;
    list->pointer++;
}

#endif
