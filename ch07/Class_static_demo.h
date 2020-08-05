#ifndef _CLASS_STATIC_DEMO_
#define _CLASS_STATIC_DEMO_

#include <string>
#include <vector>

/********************************************** 类的声明 ***************************************************/

class Account {
    // [note]: 类的静态成员是直接与类绑定，而不是与类的各个对象保持关联, 静态成员存在在任何对象之外
public:
    void calculate() { amount += amount * interestRate; }

    // [note]: 静态成员函数也不与任何对象绑定在一起, 因此不包含this指针, 也因为这个原因静态成员函数不能声明成
    //         const类型, 同时也不可以在静态成员函数内部使用this指针
    static double rate() { return interestRate; }

    // [note]: 静态成员函数定义可以在类内部定义, 也可以在外部定义, 但是在外部定义时, 不能重复static关键字, 该
    //         关键字只出现类的内部声明中
    static void rate(double);
private:
    std::string owner;
    double amount;
    // [note]: 类的静态成员不属于类的任何对象, 因此不是通过构造函数初始化的, 一般来讲需要在类的外部定义和初始化
    //         每个静态成员, 类内一般只做声明
    static double interestRate;
    static double initRate();
};

/********************************************** 其他说明 ***************************************************/
// [note]: 静态成员的使用方法
//         1. double r = Account::rate(); 使用作用域运算符访问静态成员
//         2. Account ac1;  double r = ac1.rate(); 使用类对象访问，也可以使用指针访问
//
// [note]: 静态成员可以作为默认实参, 非静态成员就不可以

#endif  /* End of file */