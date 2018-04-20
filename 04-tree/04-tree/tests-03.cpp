#include "Catch.h"
#include "tree-set.h"
#include "util.h"

/*
    Add a method named "add" to the hierarchy. The method must take
    a value to be added to the set and return a new TreeSet that contains the element.
    The original tree must remain unchanged.
    Also update Branch's size().
*/


TEST_CASE("Adding element to TreeSet<int> yields new TreeSet<int>")
{
    const auto t1 = create_empty_set<int>();
    std::shared_ptr<TreeSet<int>> t2 = t1->add(5);    
}

TEST_CASE("Adding element to TreeSet<bool> yields new TreeSet<bool>")
{
    auto t1 = create_empty_set<bool>();
    std::shared_ptr<TreeSet<bool>> t2 = t1->add(true);
}

TEST_CASE("Adding single element increments size from 0 to 1")
{
    auto t1 = create_empty_set<bool>();
    auto t2 = t1->add(true);

    CHECK(t2->size() == 1);
}

TEST_CASE("Adding leaves the original TreeSet unchanged")
{
    auto t1 = create_empty_set<char>();
    auto t2 = t1->add('a');

    CHECK(t1->size() == 0);
}

TEST_CASE("Adding multiple elements increases size by 1 at each time")
{
    auto t = create_empty_set<int>();

    CHECK(t->size() == 0);
    t = t->add(0);
    CHECK(t->size() == 1);
    t = t->add(1);
    CHECK(t->size() == 2);
    t = t->add(2);
    CHECK(t->size() == 3);
    t = t->add(3);
    CHECK(t->size() == 4);
}

TEST_CASE("Adding an element already in the set does not increment size")
{
    auto t = create_empty_set<int>();

    CHECK(t->size() == 0);
    t = t->add(0);
    CHECK(t->size() == 1);
    t = t->add(0);
    CHECK(t->size() == 1);
    t = t->add(78);
    CHECK(t->size() == 2);
    t = t->add(78);
    CHECK(t->size() == 2);
}
