#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <memory>


template<typename T>
class TreeSet
{
public:
    virtual std::shared_ptr<TreeSet<T>> add(const T&) const = 0;
    virtual bool contains(const T&) const = 0;
    virtual unsigned size() const = 0;
};

template<typename T>
class Branch : public TreeSet<T>
{
private:
    T _value;
    std::shared_ptr<TreeSet<T>> _left;
    std::shared_ptr<TreeSet<T>> _right;

public:
    Branch(T value, std::shared_ptr<TreeSet> left, std::shared_ptr<TreeSet> right) : _value(value), _left(left), _right(right) { }

    std::shared_ptr<TreeSet<T>> add(const T& x) const override
    {
        if (x < _value)
        {
            return std::make_shared<Branch>(_value, _left->add(x), _right);
        }
        else if (_value < x)
        {
            return std::make_shared<Branch>(_value, _left, _right->add(x));
        }
        else
        {
            return std::make_shared<Branch>(_value, _left, _right);
        }
    }

    bool contains(const T& x) const override
    {
        if (x < _value) return _left->contains(x);
        else if (_value < x) return _right->contains(x);
        else return true;
    }

    unsigned size() const override
    {
        return 1 + _left->size() + _right->size();
    }
};

template<typename T>
class Leaf : public TreeSet<T>
{
public:
    std::shared_ptr<TreeSet<T>> add(const T& x) const override
    {
        auto leaf = std::make_shared<Leaf>();
        return std::make_shared<Branch<T>>(x, leaf, leaf);
    }

    bool contains(const T&) const override
    {
        return false;
    }

    unsigned size() const override
    {
        return 0;
    }
};

template<typename T>
std::shared_ptr<TreeSet<T>> create_empty_set()
{
    return std::make_shared<Leaf<T>>();
}

#endif
