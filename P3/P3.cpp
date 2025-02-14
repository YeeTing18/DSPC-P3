// P3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>

using namespace std;

void thread_id_exercise() {
		int age = 23;
	cout << "Hi there!\n";

	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		cout << "hello(" << id << ")";
		cout << "hello(" << id << ")\n";
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


int main(){
	thread_id_exercise();
	shared_data_exercise();
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
