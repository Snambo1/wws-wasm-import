#ifndef ACTRALLOC_H
#define ACTRALLOC_H

void actr_free(void * ptr);

void * actr_malloc(unsigned int size);

char * actr_memory_report();
double actr_memory_usage();

#endif
