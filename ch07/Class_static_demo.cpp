#include "Class_static_demo.h"

void Account::rate(double newRate)
{
    interestRate = newRate;
}

// [note]: 类静态成员的定义和初始化
double Account::interestRate = initRate();