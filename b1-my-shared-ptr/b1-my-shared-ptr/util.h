#ifndef UTIL_H
#define UTIL_H

#include <functional>


class Helper
{
private:
    std::function<void()> on_copy;
    std::function<void()> on_destruct;

public:
    Helper(std::function<void()> on_copy, std::function<void()> on_destruct) : on_copy(on_copy), on_destruct(on_destruct) { }

    Helper(const Helper& h) : on_copy(h.on_copy), on_destruct(h.on_destruct) { on_copy(); }

    ~Helper() { on_destruct(); }
};

#endif
