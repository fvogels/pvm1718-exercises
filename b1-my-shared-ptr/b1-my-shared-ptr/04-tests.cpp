#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Define the copy constructor. It should make a copy of the fields of the given my_shared_pointer
    and it should increment the refcount by one.
*/



TEST_CASE("Copy constructor makes new pointer point to the same pointee")
{
    int* pi = new int;
    my_shared_ptr<int> p(pi);
    my_shared_ptr<int> q(p);

    REQUIRE(q.get() == pi);
    REQUIRE(q.get() == p.get());
}


TEST_CASE("Copy constructor increases refcount")
{
    int* pi = new int;
    my_shared_ptr<int> p(pi);
    REQUIRE(p.refcount() == 1);

    my_shared_ptr<int> q(p);
    REQUIRE(p.refcount() == 2);
}


TEST_CASE("Copy constructor does make make copy of pointee")
{
    int count = 0;
    Helper* h = new Helper([&count]() {++count;}, []() {});
    my_shared_ptr<Helper> p(h);
    my_shared_ptr<Helper> q(p);

    REQUIRE(count == 0);
}


TEST_CASE("Destructor decreases refcount")
{
    int* pi = new int;
    my_shared_ptr<int> p(pi);
    REQUIRE(p.refcount() == 1);

    {
        my_shared_ptr<int> q(p);
        REQUIRE(p.refcount() == 2);
    }

    REQUIRE(p.refcount() == 1);
}


TEST_CASE("Three pointers to same pointee")
{
    int destruct_count = 0;
    Helper* h = new Helper([]() {}, [&destruct_count]() {++destruct_count;});

    {
        my_shared_ptr<Helper> p(h);

        {
            my_shared_ptr<Helper> q(p);

            {
                my_shared_ptr<Helper> r(p);

                REQUIRE(p.refcount() == 3);
                REQUIRE(q.refcount() == 3);
                REQUIRE(r.refcount() == 3);
                REQUIRE(destruct_count == 0);
            }

            REQUIRE(p.refcount() == 2);
            REQUIRE(q.refcount() == 2);
            REQUIRE(destruct_count == 0);
        }

        REQUIRE(p.refcount() == 1);
        REQUIRE(destruct_count == 0);
    }

    REQUIRE(destruct_count == 1);
}


TEST_CASE("Copy constructor with nullptr")
{
    my_shared_ptr<int> p;
    my_shared_ptr<int> q(p);

    REQUIRE(p.get() == nullptr);
    REQUIRE(q.get() == nullptr);
}