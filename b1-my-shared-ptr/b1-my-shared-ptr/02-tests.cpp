#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Define a second constructor, one which takes a T* and uses it to initialize
    the shared pointer, i.e., it points to the given pointer and the refcount == 1.

    Define a method refcount() that returns the refcount of the pointee.
*/


TEST_CASE("Initializing shared ptr using existing pointer")
{
    int* pi = new int;
    my_shared_ptr<int> p(pi);

    CHECK(p.get() == pi);
}

TEST_CASE("Initializing shared ptr using existing pointer sets refcount to 1")
{
    int* pi = new int;
    my_shared_ptr<int> p(pi);

    CHECK(p.refcount() == 1);
}

TEST_CASE("Initializing shared ptr does not make a copy")
{
    int count = 0;
    Helper* h = new Helper([&count]() {++count;}, []() {});
    my_shared_ptr<Helper> p(h);

    CHECK(count == 0);
}

TEST_CASE("Initializing shared ptr with nullptr")
{
    my_shared_ptr<Helper> p(nullptr);

    CHECK(p.get() == 0);
}

