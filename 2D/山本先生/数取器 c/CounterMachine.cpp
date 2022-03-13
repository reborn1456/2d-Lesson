#include "CounterMachine.h"
#include <iostream>
using namespace std;

int Count = 0;

void AddCount(void)  //add 1 to count and printf
{
	Count++;
	cout << "Added 1 to Count." << endl;
}

void ClearCount(void)  //return count to 0 and printf
{
	Count = 0;
	cout << "Clear Count to 0. " << endl;
}

void DrawCount(void)  //print out the current count
{
	cout << "Current Count: " << Count << endl << endl;
}