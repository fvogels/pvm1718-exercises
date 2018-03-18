#include "Catch.h"
#include "linked-list.h"


TEST_CASE("Creating linked list { 1, 2 }") {
    LinkedList lst(1);
    lst.append(2);

    CHECK(lst.value() == 1);
    REQUIRE(lst.next() != nullptr);
    CHECK(lst.next()->value() == 2);
    CHECK(lst.next()->next() == nullptr);
}

TEST_CASE("Creating linked list { 4, 3, 2, 1 }") {
    LinkedList lst(4);
    lst.append(3);
    lst.append(2);
    lst.append(1);

    CHECK(lst.value() == 4);
    REQUIRE(lst.next() != nullptr);
    CHECK(lst.next()->value() == 3);
    REQUIRE(lst.next()->next() != nullptr);
    CHECK(lst.next()->next()->value() == 2);
    REQUIRE(lst.next()->next()->next() != nullptr);
    CHECK(lst.next()->next()->next()->value() == 1);
    CHECK(lst.next()->next()->next()->next() == nullptr);
}
