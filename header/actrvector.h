#ifndef ACTRVECTOR_H
#define ACTRVECTOR_H
#include "actralloc.h"

#define ACTRVECTOR_INCREMENT 8

struct ActrVector
{
    unsigned int increment;
    unsigned int count;
    unsigned int allocated;
    void **head;
};

struct ActrVector *actr_vector_init(unsigned int initialSize, unsigned int incrementSize)
{
    struct ActrVector *result = actr_malloc(sizeof(struct ActrVector));

    result->increment = incrementSize;

    if (initialSize > 0)
    {
        result->head = actr_malloc(sizeof(void *) * initialSize);
        result->allocated = initialSize;
    }

    return result;
}

int actr_vector_add(struct ActrVector *list, void *item)
{
    if (list->allocated == 0)
    {
        if (list->increment == 0)
        {
            return 0;
        }
        list->head = actr_malloc(sizeof(void *) * list->increment);
        list->allocated = list->increment;
    }
    else if (list->count >= list->allocated)
    {
        if (list->increment == 0)
        {
            return 0;
        }

        void **newHead = actr_malloc(sizeof(void *) * (list->allocated + list->increment));
        for (int i = 0; i < list->count; i++)
        {
            *(newHead + i) = *(list->head + i);
        }
        actr_free(list->head);
        list->head = newHead;
        list->allocated += list->increment;
    }
    // *(list->head + list->count++) = item;
    list->head[list->count++] = item;
    return 1;
}
void actr_vector_remove(struct ActrVector *list, unsigned int index)
{
    if (list->count > 1)
    {
        *(list->head + index) = *(list->head + list->count - 1);
    }
    list->count--;
}
#endif
