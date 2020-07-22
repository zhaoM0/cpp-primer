#include <iostream>
#include <string>
using namespace std;

const string& get_long_string(const string &strA, const string &strB)
{
    return strA.size() > strB.size() ? strA : strB;
}

const string& get_max_string(const string &strA, const string &strB)
{
    return strA > strB ? strA : strB;
}

const string& get_string_mode(const string &strA, const string &strB, const string& (*fp)(const string &, const string &))
{
    return fp(strA, strB);
}

int main()
{
    const string& (*func_pointer)(const string &, const string &);

    // 也可以写为 func_pointer = &get_long_string;
    func_pointer = get_long_string;

    string strA("hello world");
    string strB("a tensorflow learning");

    cout << func_pointer(strA, strB) << endl;

    cout << get_string_mode(strA, strB, get_max_string) << endl;
    cout << get_string_mode(strA, strB, get_long_string) << endl;

    return 0;
}