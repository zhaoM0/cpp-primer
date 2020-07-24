#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#include <string>
#include <vector>

/****************************************** 类的声明 ***********************************************/

class Screen {
public:
    // [note]: 也可以使用typedef std::string::size_type pos;
    using pos = std::string::size_type;
    Screen() = default;
    Screen(pos ht, pos wd, char c) : height(ht), width(wd), contents(ht * wd, c) {}

public:
    // [note]: 隐式内联和显式内联,不过最好在类的定义出说明函数是inline的
    char get() const { return contents[cursor]; }
    inline char get(pos ht, pos wd) const;
    Screen &move(pos r, pos c);

    // [note]: 注意这里的返回值类型，为什么不设置成void类型或者bool类型，个人理解这样做可以采用这样的
    //         表达式 myScreen.move(4, 0).set("#")
    Screen &set(char ch);
    Screen &set(pos ht, pos wd, char ch);

    // [note]: 如果想将display添加到一组动作序列中，例如 myScreen.display(cout).set('*'), 通常display
    //         不会改变Screen内容，因此需要将display定义做一个const成员, 但是这样返回的就是一个常量引用
    //         类型，也就是const Screen &, 这种类型是没有办法接着嵌套使用set的;
    // 
    // [note]: 当一个成员调用另外一个成员时, this指针是在其中隐式传递的, 当display调用do_display，它会
    //         this指针隐式的传递给do_display, 常量版本的传递的this指针是const Screen* const类型, 而非
    //         常量版本调用时传递的this指针是Screen* const
public:
    Screen &display(std::ostream &os) { do_display(os); return *this; }
    // [note]: 后置的const将this指针指向常量对象, 所以返回的引用也必须是常量引用
    const Screen &display(std::ostream &os) const { do_display(os); return *this; }
private:
    void do_display(std::ostream &os) const { os << contents; }

private:
    pos cursor = 0;
    pos height = 0;
    pos width = 0;
    std::string contents;

    // [note]: 将整个类作声明为Screen的友元,友元类的成员函数可以访问该类的所有成员
    friend class Window_mgr;
    
    // [note]: 可以将成员函数作为友元, 但这时候需要组织好依赖关系(略，后补充)
    // friend void Window_mgr::clear(ScreenIndex);
};

class Window_mgr {
public:
    using ScreenIndex = std::vector<Screen>::size_type;
    void clear(ScreenIndex);
private:
    std::vector<Screen> screens{ Screen(24, 80, ' ') };
};

/****************************************** 其他说明 ***********************************************/
// [note]: 1. 类的声明，类似函数的声明，类的声明和定义可以分离，这种先声明被称为forward declaration，但
//         这种没有完整定义，因此是一个不完全类型，这种类型只能在有限的情况下使用: 1)可以定义指向这种类型
//         的指针和引用; 2)可以声明(不可定义)以不完全类型作为参数或者返回值的函数
//         2. 只有类完成后才算被定义，所以一个类成员不可以是该类自己，因为该类还不完整，但是可以定义指向
//         本身的引用和指针,例如经典的二叉树的定义
struct BTNode {
    int data;
    struct BTNode* lchild;
    struct BTNode* rchild;
};



#endif /* End of File */