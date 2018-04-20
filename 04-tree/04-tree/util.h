#ifndef UTIL_H
#define UTIL_H

struct Foo
{
    bool operator <(const Foo& foo) const
    {
        return false;
    }
};

struct Bar
{
    int x;

    bool operator <(const Bar& bar) const
    {
        return x < bar.x;
    }
};

template<typename T, typename U>
bool has_dynamic_type(const U& x)
{
    return dynamic_cast<const T*>(&x) != nullptr;
}

template<typename T>
bool is_leaf(const TreeSet<T>& x)
{
    return has_dynamic_type<Leaf<T>>(x);
}

template<typename T>
bool is_branch(const TreeSet<T>& x)
{
    return has_dynamic_type<Branch<T>>(x);
}

#endif
