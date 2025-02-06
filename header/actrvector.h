#ifndef ACTRVECTOR_H
#define ACTRVECTOR_H
#include "actralloc.h"
#define ACTRVECTOR_INCREMENT 8
struct ActrVector
{
    int pointer;
    int length;
    void *head;
};
void actr_vector_add(struct ActrVector *list, void * item)
{
    if (list->length == 0)
    {
        list->head = actr_malloc(sizeof(void *) * ACTRVECTOR_INCREMENT);
        list->length = ACTRVECTOR_INCREMENT;
    }
    else if (list->pointer >= list->length)
    {
        void *newHead = actr_malloc(sizeof(void *) * (list->length + ACTRVECTOR_INCREMENT));
        for (int i = 0; i < list->pointer; i++)
        {
            *(void**)(newHead + i * sizeof(void*)) = *(void**)(list->head + i * sizeof(void*));
        }
        actr_free(list->head);
        list->head = newHead;
        list->length += ACTRVECTOR_INCREMENT;        
    }
    *(void**)(list->head + list->pointer++ * sizeof(void*)) = item;
}
#endif
