// P3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>
#include <stdio.h>

using namespace std;

void thread_id_exercise() {
    int x = 5;
#pragma omp parallel
    {
        x = x + 1;
        printf("shared: x is %d\n", x);
    }
}

void shared_data_exercise() {
    int x = 5;
#pragma omp parallel
    {
        x = x + 1;
        printf("shared: x is %d\n", x);
    }
}

void private_data_exercise1() {
    int x = 5;
#pragma omp parallel
    {
        //int x; x = 3;
        int x; x = omp_get_thread_num();
        printf("local: x is %d\n", x); //print multiple time bcs it is local
    }
    printf("global: x is %d\n", x); //print one time bcs it is global(at outside)
}

void private_data_exercise2() {
    int x = 5;
#pragma omp parallel private(x)
    {
        x = x + 1; //dangerous
        printf("private: x is %d\n", x);
    }
    printf("after: x is %d\n", x); //also dangerous
}

void private_data_exercise3() {
    int x = 5;
#pragma omp parallel firstprivate(x)
    {
        x = x + 1; //dangerous
        printf("private: x is %d\n", x);
    }
    printf("after: x is %d\n", x); //also dangerous
}

static long num_steps = 100000000;
double step;
double compute_pi()
{
    int i;
    double x, pi, sum = 0.0;
    double start_time, run_time;

    step = 1.0 / (double)num_steps;

    start_time = omp_get_wtime();


    for (i = 1; i <= num_steps; i++) {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }


    pi = step * sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %.16lf in %lf seconds\n ", num_steps, pi, run_time);

    return run_time;
}

double compute_pi_in_parallel_v1()
{
    double pi, sum = 0.0;
    double start_time, run_time;
    const int THREAD_SIZE = 16;
    double partial_sum[THREAD_SIZE] = { 0.0 };

    step = 1.0 / (double)num_steps;
    start_time = omp_get_wtime();

#pragma omp parallel num_threads(THREAD_SIZE)
    {
        int i;
        double x;
        int id = omp_get_thread_num();
        for (i = id + 1; i <= num_steps; i += THREAD_SIZE) {
            x = (i - 0.5) * step;
            partial_sum[id] += 4 / (1.0 + x * x);
        }
    }

    for (int j = 0; j < THREAD_SIZE; j++) {
        sum += partial_sum[j];
    }

    pi = step * sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %.16lf in %lf seconds\n ", num_steps, pi, run_time);

    return run_time;
}

double compute_pi_in_parallel_v2()
{
    double pi, sum = 0.0;
    double start_time, run_time;
    const int THREAD_SIZE = 16;
    const int cache = 64;
    double partial_sum[THREAD_SIZE][cache];

    double step = 1.0 / (double)num_steps;  // Step size

    start_time = omp_get_wtime();

    //for loop to initialize all partial_sum elements to zero
    for (int i = 0; i < THREAD_SIZE; i++) {
        for (int j = 0; j < cache; j++) {
            partial_sum[i][j] = 0.0;
        }
    }

#pragma omp parallel num_threads(THREAD_SIZE)
    {
        int i;
        double x;
        int id = omp_get_thread_num();
        for (i = id + 1; i <= num_steps; i += THREAD_SIZE) {
            x = (i - 0.5) * step;
            partial_sum[id][i % cache] += 4 / (1.0 + x * x);
        }
    }

    //for loop to total up all partial sum
    for (int j = 0; j < THREAD_SIZE; j++) {
        for (int i = 0; i < cache; i++) {
            sum += partial_sum[j][i];  
        }
    }

    pi = step * sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %.16lf in %lf seconds\n ", num_steps, pi, run_time);

    return run_time;
}

int main()
{
    //thread_id_exercise();
    //shared_data_exercise();
    //private_data_exercise1();
    //private_data_exercise2();
    //private_data_exercise3();
    //compute_pi();
    //compute_pi_in_parallel_v1();
    //compute_pi_in_parallel_v2();


    double rt[3];
	rt[0] = compute_pi();
	rt[1] = compute_pi_in_parallel_v1();
	rt[2] = compute_pi_in_parallel_v2();

    cout << endl;
	cout << "The performance gain of rt[1] is " << rt[0] / rt[1] << endl;
	cout << "The performance gain of rt[2] is " << rt[0] / rt[2] << endl;


    //int age = 23;
    //cout << "Hi there!\n";
    //#pragma omp parallel
    //{
    //    int id = omp_get_thread_num();
    //    //cout << "Hello World!\n";
    //    cout << "hello(" << id << ")";
    //    cout << "world(" << id << ")\n";
    //}
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


//z = 32;
//for (x = y - 2 * z;x < LIMIT;x += z * y) {
 //   x = 2 * y - z//        y += 2;
//}

//y = 56 * z + s;

//z = 32;
//Phase 0
//x = y - 2 * z;
//x = 2 * y - z;
//y += 2;

//Cycle 1
//x += z * y;
//x = 2 * y - z;
//y += 2;

//Cycle 2
//x += z * y;
//x = 2 * y - z;
//y += 2;

//Cycle 3
//x += z * y;
//x = 2 * y - z;
//y += 2;

//....
//y = 56 * z + s;
