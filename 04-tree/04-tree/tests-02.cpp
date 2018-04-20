#include "Catch.h"
#include "tree-set.h"
#include "util.h"

/*
    Add a method size() to your hierarchy. For now, Branch's implementation of this method
    can return a dummy value.
*/


TEST_CASE("Size of an empty TreeSet<int> is zero")
{
    const auto t = create_empty_set<int>();

    CHECK(t->size() == 0);
}

TEST_CASE("Size of an empty TreeSet<bool> is zero")
{
    const auto t = create_empty_set<bool>();

    CHECK(t->size() == 0);
}

TEST_CASE("Size of an empty TreeSet<char> is zero")
{
    const auto t = create_empty_set<char>();

    CHECK(t->size() == 0);
}

TEST_CASE("TreeSet is a polymorphic type (i.e., it contains at least one virtual member)")
{
    const auto t = create_empty_set<int>();
    CHECK(is_leaf(*t));
}
