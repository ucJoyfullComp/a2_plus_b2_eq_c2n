#include <iostream>
// #include <mutex>
#include <pthread.h>
#include <cln/integer.h>
#include <cln/io.h>
#include <cln/integer_io.h>

using namespace std;
using namespace cln;

#define NUM_THREADS 6

struct thread_data {
    int  thread_id;
    long long unsigned int n;
    int state;
};

struct thread_inst {
    pthread_t thread;
    struct thread_data td;
    int occupied;
};

pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void *FindSolutionDiophantine(void *threadarg)
{
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    cl_I a, b, c, a2, c4, cn, b2;
    long long unsigned int ic, idx;

//    cout << "thread #" << my_data->thread_id << " n=" << my_data->n << endl;
    for (ic=5; ic<1000000; ic++) {
        c = ic;
        for (cn=1, idx=0; idx<my_data->n; idx++)
            cn *= c;
        for (a=cn-1; a>0; a--) {
            a2 = a * a;
            c4 = cn * cn;
            b2 = c4 - a2;
            isqrt(b2, &b);
            if (b*b == b2) {
                pthread_mutex_lock( &count_mutex );
                cout << "Thread #" << my_data->thread_id << ", solution found: " << a << "^2 + " << b << "^2 = " << ic << "^2n, n=" << my_data->n << endl << flush;
                pthread_mutex_unlock( &count_mutex );
                my_data->state = 1;
                pthread_exit(NULL);
            }
        }
    }
    pthread_mutex_lock( &count_mutex );
    cout << "Thread #" << my_data->thread_id << ", solution not found: n=" << my_data->n << " tried until " << c << "^(2*" << my_data->n << ")=" << c4 << endl << flush;
    pthread_mutex_unlock( &count_mutex );
    my_data->state = -1;
    pthread_exit(NULL);
}

struct thread_inst *GetThread(struct thread_inst threads[], const int num)
{
    int idx;
    
    for(idx=0; idx<num; idx++) {
        if (threads[idx].td.state) {
            threads[idx].td.thread_id = idx;
            threads[idx].td.state = 0;
            threads[idx].occupied = 0;
            return &threads[idx];
        }
    }
    return (struct thread_inst *)NULL;
}

int main () {
    struct thread_inst threads[NUM_THREADS], *thrd;
    long long unsigned int n;
    int rc;
    int i;

    i = 0;
    for (i=0; i<NUM_THREADS; i++) {
        threads[i].occupied = 0;
        threads[i].td.thread_id = i;
        threads[i].td.state = -1;
        threads[i].td.n = 0;
    }
    for (n=2; n<1000; ) {
//        cerr << "n=" << n << endl;
        while (!(thrd = GetThread(threads, NUM_THREADS))) {}
//        cerr << "found unoccupied thread " << (void *)thrd << endl;
        thrd->td.n = n;
        rc = pthread_create(&thrd->thread, NULL, FindSolutionDiophantine, (void *)&thrd->td);

        if (rc) {
            pthread_mutex_lock( &count_mutex );
            cout << "Error:unable to create thread, " << rc << endl;
            pthread_mutex_unlock( &count_mutex );
        }
        else {
            n++;
        }
    }
    pthread_exit(NULL);
}
