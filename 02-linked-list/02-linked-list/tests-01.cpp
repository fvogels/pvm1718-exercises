#include "Catch.h"
#include "linked-list.h"


TEST_CASE("Creating linked list { 1 }") {
    LinkedList lst(1);

    CHECK(lst.value() == 1);
    CHECK(lst.next() == nullptr);
}

TEST_CASE("Creating linked list { 2 }") {
    LinkedList lst(2);

    CHECK(lst.value() == 2);
    CHECK(lst.next() == nullptr);
}
