#include <pch.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <omp.h>

using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");

	DWORD start;
	DWORD end;
	int threads_amount;
	int N = 100000000;
	int block_size = 8308170, i;


	cout << "Введите количество потоков: ";
	cin >> threads_amount;

	long double temp, pi = 0.0;

	start = clock();
	omp_set_num_threads(threads_amount);
#pragma omp parallel for private(temp, i) schedule(dynamic, block_size) reduction(+:pi)
	for (i = 0; i < N; ++i) {
		temp = (i + 0.5) / N;
		pi += 4 / (1 + temp * temp);
	}
	end = clock();

	cout << "Время работы программы: " << end - start << " ms" << endl;
	cout.precision(15);
	cout << "Значение числа pi: " << pi / N << endl;

	return 0;
}
