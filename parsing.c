#include "actrwasm.h"

[[clang::export_name("parseInt")]]
long long parseInt(char *s){
    long long n=strlen(s);
    long long buffer=0;
    for (long long i=n-1,j=1;i>=0;i++,j*=10)
        buffer+=j*(s[i]-'0');
    return buffer;

}

[[clang::export_name("parseFloat")]]
double parseFloat(char *s){
    long long n=strlen(s);
    double buffer=0;
    double idx=0;
    for (long long i=0;i<n;i++)
        if (s[i]=='.'){
            idx=i;
            break;
        }
    if (idx==0)
        idx=1.0;
    else
        idx=10.0*idx;
    for (long long i=0,j=idx;i<n;i++,j/=10.0)
        buffer+=(double)(s[i]-'0')*(double)j;
    return buffer;

}