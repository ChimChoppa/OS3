#include <pch.h>
#include <iostream>
#include <windows.h>
#include <time.h>

using namespace std;


DWORD WINAPI calculate(LPVOID lpParam);


unsigned long block_size = 8308170; // размер блока
unsigned long N = 100000000; //общее количество итераций
long double pi = 0.0;
int threads_amount; // число потоков
int current_position = 0; //текущая позиция потока
LPCRITICAL_SECTION section = new CRITICAL_SECTION; //критическая секция



int main()
{
	setlocale(LC_ALL, "rus");

	DWORD start;
	DWORD end;

	cout << "Введите количество потоков: ";
	cin >> threads_amount;

	HANDLE* Threads = new HANDLE[threads_amount];
	int* position = new int[threads_amount];
	InitializeCriticalSection(section);

	for (int i = 0; i < threads_amount; ++i) {
		position[i] = block_size * i;
		current_position = position[i];
		Threads[i] = CreateThread(NULL, 0, calculate, &position[i], CREATE_SUSPENDED, NULL);
	}

	start = clock();

	for (int i = 0; i < threads_amount; ++i) {
		ResumeThread(Threads[i]);
	}

	WaitForMultipleObjects(threads_amount, Threads, TRUE, INFINITE);

	end = clock();

	cout << "Время работы программы: " << end - start << " ms" << endl;

	cout.precision(15);
	cout << "Значение числа pi: " << pi / N << "\n";

	DeleteCriticalSection(section);
}


DWORD WINAPI calculate(LPVOID lpParam) {

	int* first = (int*)lpParam;
	int end = *first + block_size;
	long double x = 0.0;
	long double temp = 0.0;

	do {

		temp = 0.0;

		for (int i = *first; (i < end) && (i < int(N)); ++i) {
			x = (i + 0.5) / N;
			temp += (4 / (1 + x * x));
		}

		EnterCriticalSection(section);
		pi += temp;
		current_position += block_size;
		*first = current_position;
		LeaveCriticalSection(section);

		end = *first + block_size;

	} while (*first < int(N));

	return 0;
}
