#include "Catch.h"
#include "tree-set.h"
#include "util.h"
#include <numeric>
#include <limits>

/*
    Lastly, implement a contains method that checks whether a value is contained in the set.
*/


TEST_CASE("Empty TreeSet does not contain any value")
{
    const auto t = create_empty_set<bool>();
    
    CHECK(!t->contains(true));
    CHECK(!t->contains(false));
}

TEST_CASE("Adding element causes contains to return true")
{
    auto t = create_empty_set<Foo>();
    Foo foo;

    CHECK(!t->contains(foo));
    t = t->add(foo);
    CHECK(t->contains(foo));
}

TEST_CASE("TreeSet contains only elements that are added")
{
    auto t = create_empty_set<uint8_t>();

    for (uint16_t i = 0; i < std::numeric_limits<uint8_t>::max(); ++i)
    {
        REQUIRE(!t->contains(uint8_t(i)));
    }

    t = t->add(0);

    t->contains(0);
    for (uint16_t i = 1; i < std::numeric_limits<uint8_t>::max(); ++i)
    {
        REQUIRE(!t->contains(uint8_t(i)));
    }
}

TEST_CASE("TreeSet<Foo> contains element that is added multiple times")
{
    auto t = create_empty_set<Foo>();
    Foo foo;

    t = t->add(foo);
    t = t->add(foo);

    CHECK(t->contains(foo));
}

TEST_CASE("TreeSet<int> contains element that is added multiple times")
{
    auto t = create_empty_set<int>();

    t = t->add(8);
    t = t->add(8);
    t = t->add(8);

    CHECK(t->contains(8));
}

TEST_CASE("TreeSet should make copies of elements")
{
    auto t = create_empty_set<Bar>();
    Bar bar{ 5 };

    t = t->add(bar);
    bar.x++;

    CHECK(!t->contains(bar));
}
