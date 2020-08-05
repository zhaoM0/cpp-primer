#include <memory>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// [note]: 多对象共享一个相同的存储区域示例
//         这里采用默认版本的拷贝, 赋值和销毁成员函数来对此类类型的对象操作
class StrBlob {
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    // 添加删除元素
    void push_back(const std::string &t) { data->push_back(t); }
    void pop_back();
    // 元素访问
    std::string &front();
    std::string &back();
private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i, const std::string &msg) const;
};

StrBlob::StrBlob() : data(make_shared<vector<string>>()) { }
StrBlob::StrBlob(initializer_list<string> il) : data(make_shared<vector<string>>(il)) { }

void StrBlob::check(size_type i, const string &msg) const
{
    if(i >= data->size())
        throw out_of_range(msg);
}

string &StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

string &StrBlob::back()
{
    check(0, "back ont empty StrBlob");
    return data->back();
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlod");
    data->pop_back();
}

void test_allocator_demo();


int main()
{
    // [note]: 内存池
    //         ----------------------------------------------------------------------------
    //         -  静态内存              -  局部static对象, 类static成员, 函数之外的变量    -
    //         -  栈内存                -  函数内定义的非static成员,自动创建和销毁         -
    //         -  动态内存/堆           -  动态分配的对象(new和delete管理的对象)           -
    //         ----------------------------------------------------------------------------
    //
    // [note]: 智能指针(smart pointer)
    //         用于管理动态对象,主要有两种智能指针:1) shared_ptr 2) unique_ptr, 定义在memory
    //
    // [note]: share_ptr -- 允许多个指针指向同一个对象
    //         make_shared是标准库函数, 调用后在动态内存中分配一个对象并初始化,返回一个指向对象
    //         的shared_ptr, 这里的操作类似顺序容器的emplace, 传递的参数必须与对应类的某个构造
    //         函数相匹配
    shared_ptr<int> p1 = make_shared<int>(199);
    shared_ptr<string> p2 = make_shared<string>(10, '9');
    shared_ptr<int> p3 = make_shared<int>();                // 这里采用值初始化      
    cout << *p1 << " " << *p2 << " " << *p3 << endl;

    // [note]: shared_ptr拷贝和赋值
    //         可以认为每个shared_ptr都包含一个关联计数器, 用于记录有多少个指针共享对象,下例中
    //         递增p1指向的对象的引用计数, 递减r指向对象的引用计数, 由于r指向对象没有引用者,就
    //         会自动释放
    auto r = make_shared<int>(42);
    r = p1;

    // [note]: 使用动态内存的三种情况
    //         1. 程序不知道自己使用需要使用多少对象 -- 容器类
    //         2. 程序不知道所需对象的准确类型 -- 略
    //         3. 程序需要在多个对象间共享数据 -- 可能会有多个对象共享底层数据,一个对象小时候不
    //                                           能单方面销毁底层数据,使用动态内存允许多个对象
    //                                           共享相同的状态
    StrBlob b1;
    {
        StrBlob b2 = {"a", "b", "c", "d"};
        b1 = b2;
        b2.push_back("end");
    }
    cout << b1.size() << endl;

    // [note]: 直接管理动态内存 -- new和delete运算符
    //         直接管理内存的类不可以依赖类的默认的对象拷贝，赋值和销毁操作


    test_allocator_demo();
}

void test_allocator_demo()
{
    // [note]: new存在灵活性的缺陷, 它将内存分配和对象构造组合在一起, delete类似, 将析构和内存释
    //         放组合在一起, 下例中一开始申请了10string所需要的内存, 并均默认初始化, 但是后续的
    //         部分只用到5个string, 这样就创建了5个永远用不到的对象，而且即使是使用的前5个对象也
    //         是被赋值了两次, 一次new时候的默认初始化, 一次是输入后的赋值
    string *const p = new string[10];
    string *q = p;
    string tmp;
    for(size_t i = 0; i < 5; i++) {
        cin >> tmp;
        *q++ = tmp;
    }
    const size_t size = q - p;
    delete[] p;

    // [note]: alloctor是对malloc的封装, 它分配的内存是原生的未构造的内存, 使用allocotr的流程如下
    //         allocate(分配内存) -> construct(构造) -> destroy(析构) -> deallocate(去配)
    //
    //         --------------------------------------------------------------------------------        
    //         -  allocator<T> ac          -  定义一个为T分配对象的allocator对象               -
    //         -  ac.allocate(n)           -  分配一个n个保存T的对象的内存                     -
    //         -  ac.deallocate(p, n)      -  p必须是allocate返回的指针, 且n必须为p创建时大小  -
    //         -                           -  (注意deallocate之前内存所有对象必须destroy)      -
    //         -  ac.construct(p, args)    -  p必须是T*的指针,arg用来构造对象,类似emplace      -
    //         -  ac.destroy(p)            -  对p指向的对象进行析构操作                        -
    //         --------------------------------------------------------------------------------
    allocator<string> alloc;
    auto const p1 = alloc.allocate(10);
    auto q1 = p1;
    alloc.construct(q1++);
    alloc.construct(q1++, 10, 'A');
    alloc.construct(q1++, "hello world");
    cout << *p1 << endl;
    // cout << *q1 << endl;        // 灾难，未构造的内存

    // [note]: 1. 使用没有construct的对象, 其行为是未定义的
    //         2. 我们只能对真正构造了的元素进行destroy操作
    // 
    // [prob]: string本身的就是可变长度的指针, 如果使用allocate分配10个空的string, 在使用的时候，如
    //         果某个对象的长度变得非常大，引起内存的重新分配，导致分配的内存不够，这个问题allocotor
    //         是如何解决的？
    while(q1 != p1)
        alloc.destroy(q1--);
    alloc.deallocate(p1, 10);

    // [note]: 标准库为allocator类定义的两个伴随算法
    //         ---------------------------------------------------------------------------------
    //         -  uninitialized_copy(b, e, to_b)     - [b, e)拷贝到to_b指定的未构造的内存       -
    //         -  uninitialized_copy_n(b, n, to_b)   - 从b开始, 拷贝n个元素到to_b指向的内存     -
    //         -  uninitialized_fill(b, e, cp_obj)   - [b, e)创建对象，均为cp_obj的拷贝         -
    //         -  uninitialized_fill_n(b, n, cp_obj) - 从b开始创建n个对象, 均为cp_obj的拷贝     -
    //         ---------------------------------------------------------------------------------
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    allocator<int> al_int;
    auto two_len_arr = al_int.allocate(arr.size() * 2);
    auto mid_ptr = uninitialized_copy(arr.begin(), arr.end(), two_len_arr);
    auto end_ptr = uninitialized_fill_n(mid_ptr, arr.size(), 10);
    int* head = two_len_arr;
    while(head != end_ptr)
        cout << *head++ << " ";
    cout << endl;
}

void test_dynamic_array_demo()
{
    // [note]: 使用new和delete分配的动态数组的类不可以使用默认的拷贝,赋值和析构函数,而使用容器的
    //         类可以使用默认版本的这些函数
    // 
    // [note]: 两种写法,但是编译器最后都会转化new int[10]这种形式
    //         1. int *pi_1 = new int[10];
    //         2. typedef int arrT[10]; int *pi_2 = new arrT;
    //
    // [note]: 动态数组并不是数组类型,使用new T[]并不会返回一个数组类型的对象,而是T*类型的指针,
    //         因为不是数组类型,所以不可以使用begin和end,也不可以使用范围for来处理动态数组元素
    //
    // [note]: 使用new运算符创建的对象,都是采用默认的初始化的,但是可以指定初始化和列表初始化
    int *pi_1 = new int[10];                                  // 默认初始化
    int *pi_2 = new int[10]();                                // 值初始化
    int *pi_3 = new int[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};    // 列表初始化
    int *pi_4 = new int[10]{2, 3, 4};                         // 剩余元素采用值初始化

    // [note]: delete一个数组指针的时候没有加[],或者delete一个单一对象使用[],均会导致程序异常
    delete[] pi_1;
    delete[] pi_2;
    delete[] pi_3;
    delete[] pi_4;

    // [note]: 智能指针与动态数组
    // 
}