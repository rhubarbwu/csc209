#include <stdio.h>

int main() {

    // Try running this code in the visualizer and then try playing with the types
    // using chars instead of ints.
    
    int A[3] = {13, 55, 20};
    int *p = A;

    // dereference p
    printf("%d\n", *p);

    // When we add an integer to a pointer, the result is a pointer.
    int *s;
    s = p + 1;

    // dereference s to see where this new pointer points
    printf("%d\n", *s);

    // first add 1 to p and dereference the result
    printf("%d\n", *(p+1));

    // we can use array syntax on p
    printf("%d\n", p[0]);
    printf("%d\n", p[1]);

    p = p + 1;    
    printf("%d\n", *p);
    printf("%d\n", p[0]);
    printf("%d\n", p[1]);
    printf("%d\n", *(p - 1));

    return 0;

}