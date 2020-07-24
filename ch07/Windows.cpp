#include "Windows.h"

inline Screen &Screen::move(pos r, pos c)
{
    cursor = r * width + c;
    return *this;
}

inline char Screen::get(pos ht, pos wd) const
{
    return contents[ht * width + wd];
}

inline Screen &Screen::set(char ch)
{
    contents[cursor] = ch;
    return *this;
}

inline Screen &Screen::set(pos ht, pos wd, char ch)
{
    contents[ht * width + wd] = ch;
    return *this;
}

void Window_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];
    s.contents = std::string(s.height * s.width, ' ');
}