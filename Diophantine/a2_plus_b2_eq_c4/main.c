#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
    long long unsigned int a, b, c, c4, a2;
    double db;
    /* a^2+b^2=c^4 */
    
    for (c=2; c<60000; c++) {
        printf("c=%llu\n", c);
        for (a=1; a<c; a++) {
            c4 = c*c*c*c;
            a2 = a*a;
            db = floor(sqrt(c4-a2));
            b = (long long unsigned int)db;
            if ((a2+b*b==c4) || (a2+(b+1)*(b+1)==c4)) {
                printf("solution: %llu^2+%llu^2=%llu^4\n", a, b, c);
                return 0;
            }
        }
    }
	return 1;
}
