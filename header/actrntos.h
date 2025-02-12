#ifndef ACTRNTOS_H
#define ACTRNTOS_H
#include "actrwasm.h"

void reverse(char *str, int len)
{
    int i = 0, j = len - 1;
    while (i < j)
    {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int _itos(long long value, char *string)
{
    int i = 0;
    int isNeg = 0;
    if (value == 0) {
        string[i++] = 48;
        return i;
    }
    if (value < 0)
    {
        isNeg = 1;
        value = -value;
    }

    while (value != 0)
    {
        long long rem = value % 10;
        value /= 10;
        string[i++] = rem + 48;
    }
    if (isNeg)
    {
        string[i++] = '-';
    }
    reverse(string, i);
    return i;
}
long long pow(long long base, int exponent)
{
    long long result = base;
    for (int i = 1; i < exponent; i++)
    {
        result *= base;
    }
    return result;
}
char *itos(long long value)
{
    char buffer[256];
    int i = _itos(value, buffer);
    char *result = actr_malloc(i + 1);

    for (int j = 0; j < i; j++)
    {
        result[j] = buffer[j];
    }
    return result;
}
char *ftos(double value, int precision)
{
    long long ipart = (long long)value;
    double fpart = value - ipart;

    char buffer[256];

    int i = _itos(ipart, buffer);

    if (precision != 0)
    {
        buffer[i++] = '.';
        fpart = fpart * pow(10, precision);
        if (fpart < 0)
        {
            fpart = -fpart;
        }

        i += _itos((long long)fpart, buffer + i);
    }

    char *result = actr_malloc(i + 1);

    for (int j = 0; j < i; j++)
    {
        result[j] = buffer[j];
    }

    return result;
}

#endif
