#include "math.h"
#include "stdio.h"

int as_int(int i) {
    return i;
}

long as_long(long l) {
    return l;
}

double as_double(double f) {
    return f;
}

float as_float(float d) {
    return d;
}

int main() {
    int nine_plus_ten = 21;
    long massive = __LONG_MAX__ - 1;  // wrap to the negatives
    printf("%d -> %ld\n", nine_plus_ten, as_long(nine_plus_ten));
    printf("%ld -> %d\n", massive, as_int(massive));

    float zero = 0.0;
    double pi = M_PI;  // approximate the approximation
    printf("%1.32f -> %1.32f\n", zero, as_double(zero));
    printf("%1.32f -> %1.32f\n", pi / 2, as_float(pi) / 2);

    return 0;
}