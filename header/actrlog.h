#ifndef ACTRLOG_H
#define ACTRLOG_H
#include "actrwasm.h"
void actr_log_length(const char * pointer, unsigned int size);
void actr_log(const char * pointer) {
    actr_log_length(pointer, strlen(pointer));
}
#endif
