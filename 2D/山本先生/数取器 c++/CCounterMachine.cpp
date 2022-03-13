#include "CCounterMachine.h"
#include <iostream>
using namespace std;

CCounterMachine::CCounterMachine()
:mCount(0)   //‰Šú‰»qƒŠƒXƒg
{
}

CCounterMachine::~CCounterMachine()
{

}

void CCounterMachine::AddCount(void)  //add 1 to count and printf
{
	mCount += 1;
	cout << "Added 1 to Count." << endl;
	DrawCount();
}

void CCounterMachine::ClearCount(void)  //return count to 0 and printf
{
	mCount = 0;
	cout << "Clear Count to 0. " << endl;
	DrawCount();
}

void CCounterMachine::DrawCount(void)  //print out the current count
{
	cout << "Current Count: " << mCount << endl << endl;
}

const int CCounterMachine::GetCount()
{
	return mCount;
}