#include <stdio.h>

int main() {
    int x=5; //removed
    x=15; //safe because of line 9
    int y=6; //removed
    y=x; //removed
    y=3; 
    printf("%d\n", x);
    x=1; //removed
    x=10;
    return 0;
}

