#include "Catch.h"
#include "linked-list.h"


TEST_CASE("Length of {1}")
{
    const LinkedList lst(1);

    CHECK(lst.length() == 1);
}

TEST_CASE("Length of {2, 2}")
{
    LinkedList lst(2);
    lst.append(2);

    CHECK(lst.length() == 2);
}

TEST_CASE("Length of {1, 3, 5}")
{
    LinkedList lst(1);
    lst.append(3);
    lst.append(5);

    CHECK(lst.length() == 3);
}

TEST_CASE("Length of {9, 7, 5, 3, 1, 2, 4, 6, 8}")
{
    LinkedList lst(9);
    lst.append(7);
    lst.append(5);
    lst.append(3);
    lst.append(1);
    lst.append(2);
    lst.append(4);
    lst.append(6);
    lst.append(8);

    CHECK(lst.length() == 9);
}
