#include <iostream>
#include <cln/integer.h>
#include <cln/io.h>
#include <cln/integer_io.h>

using namespace std;
using namespace cln;

int main(int argc, char **argv)
{
    cl_I a, b, c, a2, c4, cn, b2;
    long long unsigned int ic, n, idx;
    int flag;
    
    for (n=2; n<1000; n++) {
//        for (flag=-1, ic=2; ic<1000000 && flag; ic++) {
        for (flag=-1, ic=5; ic<1000000 && flag; ic++) {
            c = ic;
            for (cn=1, idx=0; idx<n; idx++)
                cn *= c;
            for (a=cn-1; a>0; a--) {
                a2 = a * a;
                c4 = cn * cn;
                b2 = c4 - a2;
                isqrt(b2, &b);
                if (b*b == b2) {
                    cout << "solution found: " << a << "^2 + " << b << "^2 = " << ic << "^2n, n=" << n << endl;
                    flag = 0;
                    break;
                }
            }
        }
    }
	return 1;
}
