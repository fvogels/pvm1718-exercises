#include "Catch.h"
#include "tree-set.h"
#include "util.h"

/*
    Start by defining the three classes TreeSet, Branch and Leaf. For now, you can leave them empty.

    Define a function create_empty_set<T> that returns an empty TreeSet<T>, i.e., a Leaf<T>.
*/


TEST_CASE("create_empty_set<int>() returns a std::shared_ptr<TreeSet<int>>")
{
    std::shared_ptr<TreeSet<int>> t = create_empty_set<int>();
}

TEST_CASE("create_empty_set<double>() returns a std::shared_ptr<TreeSet<double>>")
{
    std::shared_ptr<TreeSet<double>> t = create_empty_set<double>();
}

TEST_CASE("create_empty_set<Foo>() returns a std::shared_ptr<TreeSet<Foo>>")
{
    std::shared_ptr<TreeSet<Foo>> t = create_empty_set<Foo>();
}
