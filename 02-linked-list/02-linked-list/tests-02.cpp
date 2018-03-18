#include "Catch.h"
#include "linked-list.h"


TEST_CASE("Const correctness of value(): testing on nonconst object")
{
    LinkedList lst(5);

    CHECK(lst.value() == 5);
}

TEST_CASE("Const correctness of value(): testing on const object")
{
    const LinkedList lst(5);

    CHECK(lst.value() == 5);
}

TEST_CASE("Const correctness of next(): testing on nonconst object")
{
    LinkedList lst(5);

    CHECK(lst.next() == nullptr);
}

TEST_CASE("Const correctness of next(): testing on const object")
{
    const LinkedList lst(5);

    CHECK(lst.next() == nullptr);
}
