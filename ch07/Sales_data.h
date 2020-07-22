#ifndef _SALES_DATA_H_
#define _SALES_DATA_H_

#include <iostream>
#include <string>
#include <vector>

// [note]: 定义类时，需要控制初始化, 赋值, 拷贝, 销毁对象时的行为

/****************************************  类的定义  *****************************************/
class Sales_data {
public:
    // [note]: 关于默认构造函数
    //         1. "=default"要求编译器生成默认的构造函数,默认构造函数无需任何实参，合成的默认的函数
    //            初始类数据成员，如果有类内初始值（有的编译器可能不支持），先初始化，否则执行默认
    //            初始化;
    //         2. 一旦定义了构造函数，则编译器不会生成默认的构造函数；若类内包含指针或者符合类型可能
    //            会出现一些错误;
    Sales_data() = default;

    // [note]: 构造函数的初始值列表(constructor initialize list)
    Sales_data(const std::string &s, unsigned n, double p) : 
            bookNo(s), units_sold(n), revenue(p * n) {}
    Sales_data(const std::string &s) : bookNo(s) {}

    Sales_data(std::istream &);

    // [note]: 关于this指针 -- 与python类中self呼应
    //         1. 若采用total.isbn(), 这里将total的指针传递给this指针,过程类似isbn(&total),将this
    //            指针当作调用对象统一的整体使用;
    //         2. this指针的类型 -- 常量指针
    //            class_name * const this; 这里this一旦绑定对象就不可以再绑定其他对象
    //         3. isbn()后面的const是将this指针声明为指向常量的指针,即const Sales_data *const
    std::string isbn() const { return bookNo; }

    // [note]: 这个函数的设计初衷类似于运算符 "+=",且注意这个函数的返回值
    Sales_data &combine(const Sales_data &);

private:
    double avg_price() const { return units_sold ? revenue / units_sold : 0; }
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0;

    // [note]: 友元的说明 -- 接口的一部分，但是不属于类成员
    //         在类内声明函数或者类作为其友元，这样的函数或者类就可以访问该类的非共有成员了,这里的
    //         友元声明仅仅是指定了函数的访问权限，不同于通常意义的函数声明，需要提供独立声明
    friend Sales_data add(const Sales_data &, const Sales_data &);
    friend std::ostream &print(std::ostream &, const Sales_data &);
    friend std::istream &read(std::istream &, Sales_data &);
};

/****************************************  相关接口  *****************************************/
// [note]: 这里最好提供独立的声明
Sales_data add(const Sales_data &, const Sales_data &);

// [note]: 1.IO类属于不能拷贝的类型，只能使用引用来传递,且读写会改变流的内容，不能够是常量引用
std::ostream &print(std::ostream &, const Sales_data &);
std::istream &read(std::istream &, Sales_data &);

#endif  /* End of file */