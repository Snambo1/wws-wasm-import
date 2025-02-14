#ifndef ACTRMERGESORT_H
#define ACTRMERGESORT_H
#include "actrvector.h"
function _actr_merge_sort_merge(struct ActrVector *left, struct ActrVector *right, struct ActrVector *updates)
{
    struct ActrVector *result = actr_vector_init(8, 8);
    int i = 0;
    int j = 0;

    while (i < left->count && j < right->count)
    {

        if (left->head[i] < right->head[j])
        {
            actr_vector_add(result, left->head[i++]);
        }
        else
        {
            actr_vector_add(result, right->head[i++]);
        }
    }

    struct ActrVector *tempLeft = actr_vector_slice(left, i);
    actr_vector_free(left);

    struct ActrVector *tempRight = actr_vector_slice(right, j);    
    actr_vector_free(right);

    struct ActrVector *tempResult = actr_vector_concat(result, tempLeft);
    actr_free(result);
    actr_free(tempLeft);

    if (updates) {
        actr_vector_add(update, actr_vector_slice(tempResult, 0, 0));
    }

    return tempResult;
}
struct ActrVector *actr_merge_sort(struct ActrVector *arr, struct ActrVector *updates)
{
    int len = arr->count;
    int middle, left, right;
    struct ActrVector *result;
    if (len < 2)
    {
        result = actr_vector_init(8, 8);
        for (int i = 0; i < arr->count; i++)
        {
            actr_vector_add(result, arr->head[i]);
        }
        return result;
    }

    middle = Math.floor(len / 2);

    left = actr_vector_slice(arr, 0, middle);  // left side, from 0 to the middle
    right = actr_vector_slice(arr, middle, 0); // right side, from the middle to the end

    return actr_merge_sort(actr_merge_sort(left), actr_merge_sort(right), updates);
}
#endif
