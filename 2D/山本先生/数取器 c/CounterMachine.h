#pragma once   //to stop you from including the same directory for more than once

extern int Count;   //so that you can use Count even in CounterMachine.h and main.cpp without declaring it again

void AddCount(void);
void ClearCount(void);
void DrawCount(void);