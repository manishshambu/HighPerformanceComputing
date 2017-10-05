/**
 * @file
 * @author Rahimian, Abtin <arahimian@acm.org>
 * @revision $Revision$
 * @tags $Tags$
 * @date $Date$
 *
 * @brief Demonstration of OpenMP language features
 */

#ifdef _OPENMP
#include <omp.h>
#define NOW()(omp_get_wtime())
#else
#define NOW() 0
#define omp_get_thread_num() 0
#endif

#include <iostream>
#include <unistd.h> //for sleep
#define  PRINT_HEADER(msg) printf("\n\nRunning %s\n---\n%s",__FUNCTION__,msg)
#define  TID(msg) printf("Thread id=%d: %s\n",omp_get_thread_num(),msg)

int demo_parallel(){
    PRINT_HEADER("Parallel section creates threads and \nall of them execute that region\n\n");
    TID("sequential section");

    #pragma omp parallel
    {
        TID("parallel section");
    } //parallel
}

int demo_workshare(){
    PRINT_HEADER("Main work-share mechanism is omp for\n");

    // schedule(static|dynamic|guided|runtime[,chunk_size])
    int nt=omp_get_max_threads();
    int work=2;
    int sz=work*nt;
    int chunk=work;
    printf("nt=%d, sz=%d\n",nt,sz);

    #pragma omp parallel
    {
        int id(omp_get_thread_num());
        #pragma omp for schedule(static)
        for(int i(0);i<sz;++i){
            printf("Thread id=%d, i=%d\n",id,i);
            for (int j(0);j<i;++j)
                usleep(1e5);
        }
    } //parallel
}

int demo_nowait(){

    int nt(omp_get_max_threads());

    #pragma omp parallel
    {
        #pragma omp for nowait
        for(int i(0);i<nt;++i){
            usleep(i*1e5);
            printf("- exit id=%d,i=%d\n",omp_get_thread_num(),1);
        }
        printf("- middle id=%d,i=%d\n",omp_get_thread_num(),1);
        #pragma omp for
        for(int i(0);i<nt;++i){
            printf("--- enter id=%d,i=%d\n",omp_get_thread_num(),i);
            usleep(i*1e5);
        }
        printf("Thread id=%d done\n",omp_get_thread_num());
    } //parallel
}

int demo_shared_private(){
    PRINT_HEADER("Variables could be declared shared or private. \nThe inital value of private is undefined\n\n");


    int a(0),c(-4);
    #pragma omp parallel shared(a) private(c)
    {
        int b(a);
        a=omp_get_thread_num();
        printf("Thread id=%d: a=%d,b=%d,c=%d\n",omp_get_thread_num(),a,b,c);
        c=omp_get_thread_num();
    } //parallel
    printf("a=%d,c=%d\n",a,c);
}

int demo_firstprivate(){
    PRINT_HEADER("Initializes the value of private (has overhead)");

    int a=-1;
    int n(omp_get_max_threads());
    #pragma omp parallel firstprivate(a)
    {
        printf("Thread id=%d: a=%d\n",omp_get_thread_num(),a);
        a=omp_get_thread_num();
    } //parallel
    printf("a=%d\n",a);
}

int demo_barrier(){
    PRINT_HEADER("Thread coordination, avoid race condition\n");

    int nt(omp_get_max_threads());
    #pragma omp parallel
    {
        TID("starting");
        int id(omp_get_thread_num());
        usleep(1.0e6*(nt-id));

        TID("reached barrier");
        #pragma omp barrier
        TID("after barrier");
    }
}

int demo_critical(){
    PRINT_HEADER("Critical sections are executed by one thread at a time");

    double tic(NOW());

    #pragma omp parallel
    {
        int id(omp_get_thread_num());
        TID("staring");

        #pragma omp critical (cname)
        {
            TID("critical work");
            sleep(1*(id+1));
        }

        double toc(NOW());
        printf("Thread id=%d: elapsed=%5.2e\n",id,toc-tic);
    }
}

int demo_reduction(){
    PRINT_HEADER("Cumulative sum of an array of integers\n");

    size_t n(1000);
    int *x(new int[n]);

    for (size_t ii(0);ii<n;++ii)
        x[ii]=ii;

    size_t sum(0), par_sum(0);
    #pragma omp parallel private(par_sum) shared(sum)
    {
        #pragma omp for
        for (size_t ii(0);ii<n;++ii)
            par_sum+=x[ii];

        #pragma omp atomic
        sum = sum+par_sum;
    }

    size_t ref(n*(n-1)/2);
    printf("Sum of 1-%d is %d (expected=%d)\n",n,sum,ref);
    delete[] x;
}

int foo(int i){
    TID("foo");
    usleep(1.0e6);
    return i;
}

int demo_atomic(){
    PRINT_HEADER("Atomic updates a memory location atomically");

    size_t total(0);
    size_t n(omp_get_max_threads());

    #pragma omp parallel for shared(total)
    for (size_t ii(0);ii<n;++ii)
    {
        //compare to critical
        total += foo(ii);
        TID("done");
    }

    printf("Total=%d (expected %d)\n",total,n*(n-1)/2);
}

int demo_lock(){
    PRINT_HEADER("Use locking to get access to shared resource");

    omp_lock_t l;
    omp_init_lock(&l);
    double tic(NOW());

    #pragma omp parallel
    {
        TID("starting");

        if (false) { /* test_lock */
            while(!omp_test_lock(&l)){
                printf("Thread id=%d: waiting\n",omp_get_thread_num());
                usleep(5e5);
            }}

        omp_set_lock(&l);
        TID("has lock");
        usleep(1e6);
        omp_unset_lock(&l);

        double toc(NOW());
        printf("Thread id=%d: elapsed=%5.2e\n",omp_get_thread_num(), toc-tic);
    }
    omp_destroy_lock(&l);
}

long fib(long n) {
    long i(0),j(0);
    if (n<2) return 1;

    #pragma omp task shared(i)
    i = fib(n-1);

    #pragma omp task shared(j)
    j = fib(n-2);

    #pragma omp taskwait
    return i+j;
}

void demo_task(){

    long n(11),v;
    #pragma omp parallel shared (n,v)
    {
        #pragma omp single
        v = fib(n);
    }
    printf("Fib(%d)=%d\n",n,v);
}

int main(int argc, char** argv){

    if (argc<2){
        printf("You need to pass the test id (1-11)\n");
        exit(1);
    }
    int testid = std::stoi(argv[1]);
    switch (testid) {
        case 1:     demo_parallel();         break;
        case 2:     demo_workshare();        break;
        case 3:     demo_nowait();           break;
        case 4:     demo_shared_private();   break;
        case 5:     demo_barrier();          break;
        case 6:     demo_critical();         break;
        case 7:     demo_reduction();        break;
        case 8:     demo_firstprivate();     break;
        case 9:     demo_atomic();           break;
        case 10:    demo_lock();             break;
        case 11:    demo_task();             break;
        }
}
