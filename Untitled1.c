#include <stdlib.h>
int test(void *thing) {
    return (int)thing;
}
int main() {
    char *what = malloc(5);
    return test(what);
    return 0;
}
