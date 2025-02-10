#ifndef ACTRLOG_H
#define ACTRLOG_H
#include "actrwasm.h"

/// @brief use actr_log instead
/// @param pointer 
/// @param size 
extern void actr_log_length(const char * pointer, unsigned long size);

/// @brief used for debugging, log messages will appear in the browser console
/// @param pointer 
void actr_log(const char * pointer) {
    actr_log_length(pointer, strlen(pointer));
}
#endif
