#ifndef ACTRSTRING_H
#define ACTRSTRING_H


unsigned long strlen(const char * string) {
    int result = 0;
    while (*string++) {
        result++;
    }
    return result;
}
#endif
