#ifndef ACTRALLOC_H
#define ACTRALLOC_H

void actr_free(void * ptr);

void * actr_malloc(unsigned int size);

/// @brief returns a simple usage memory report
/// @return allocated string must be freed 
extern char * actr_memory_report();
double actr_memory_usage();

#endif
