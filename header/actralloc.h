#ifndef ACTRALLOC_H
#define ACTRALLOC_H

extern void actr_free(void *ptr);
extern void actr_non_zero_mem();
extern void *_actr_malloc(int size);
void *actr_malloc(int size)
{
    char *result = _actr_malloc(size);
    for (int i = 0; i < size; i++)
    {
        if (result[i] != 0)
        {
            actr_non_zero_mem();
        }
    }
    return result;
}



/// @brief returns a simple usage memory report
/// @return allocated string must be freed
extern char *actr_memory_report();
extern double actr_memory_usage();

#endif
