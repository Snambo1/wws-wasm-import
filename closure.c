#include <stdio.h>

int main() {
    int x = 10;

    // Define a block (closure) that captures the variable 'x'
    int (^my_closure)(int) = ^(int y) {
        return x + y;
    };

    // Call the closure
    int result = my_closure(5); 
    printf("Result: %d\n", result); // Output: Result: 15

    x = 20;
    result = my_closure(5);
    printf("Result: %d\n", result); // Output: Result: 25

    return 0;
}