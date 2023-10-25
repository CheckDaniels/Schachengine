#include <Windows.h>
#include <stdio.h>

LARGE_INTEGER start1, end1, frequency1;
double ms;

void start(){
    QueryPerformanceFrequency(&frequency1);
    QueryPerformanceCounter(&start1);
}
void stop(){
    QueryPerformanceCounter(&end1);
    ms = (double)(end1.QuadPart - start1.QuadPart) * 1000.0 / frequency1.QuadPart;
    printf("Zeit fuer die Ausfuehrung: %f Millisekunden\n", ms);
}