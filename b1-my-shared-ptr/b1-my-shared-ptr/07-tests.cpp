#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Overload the operator =.
    When overwriting one my_shared_ptr with another one (p = q), the following steps
    need to be taken:

    *    p loses its link to its current pointee. So, it must first decrease
         its refcount, check if it reaches 0, and if so, free the pointee and refcount.
         In other words, this steps mimics the destructor.

    *    p then copies q's members and increases the refcount. This mimics the copy constructor.
*/



TEST_CASE("Assigning p = q makes p point to same pointee as q")
{
    int* i = new int;
    my_shared_ptr<int> p;
    my_shared_ptr<int> q(i);

    REQUIRE(p.get() == 0);
    REQUIRE(q.get() == i);
    REQUIRE(q.refcount() == 1);

    p = q;

    REQUIRE(p.get() == i);
    REQUIRE(q.get() == i);
    REQUIRE(p.refcount() == 2);
    REQUIRE(q.refcount() == 2);
}

TEST_CASE("Assigning p = q with p != nullptr, q == nullptr")
{
    int destruct_count = 0;
    Helper* h = new Helper([]() {}, [&destruct_count]() { destruct_count++; });

    my_shared_ptr<Helper> p(h);
    my_shared_ptr<Helper> q;

    REQUIRE(destruct_count == 0);
    REQUIRE(p.get() == h);
    REQUIRE(q.get() == nullptr);
    
    p = q;

    REQUIRE(destruct_count == 1);
    REQUIRE(p.get() == nullptr);
    REQUIRE(q.get() == nullptr);
}

TEST_CASE("Assigning p = q with p -= nullptr, q == nullptr")
{
    my_shared_ptr<Helper> p;
    my_shared_ptr<Helper> q;

    REQUIRE(p.get() == nullptr);
    REQUIRE(q.get() == nullptr);

    p = q;

    REQUIRE(p.get() == nullptr);
    REQUIRE(q.get() == nullptr);
}

TEST_CASE("Assigning p = q decreases refcount of p, increases that of q")
{
    int* i = new int;
    int* j = new int;

    my_shared_ptr<int> pi(i);
    my_shared_ptr<int> qi(pi);
    my_shared_ptr<int> ri(pi);

    my_shared_ptr<int> pj(j);
    my_shared_ptr<int> qj(pj);
    my_shared_ptr<int> rj(pj);

    REQUIRE(pi.get() == i);
    REQUIRE(qi.get() == i);
    REQUIRE(ri.get() == i);
    REQUIRE(pi.refcount() == 3);
    REQUIRE(qi.refcount() == 3);
    REQUIRE(ri.refcount() == 3);

    REQUIRE(pj.get() == j);
    REQUIRE(qj.get() == j);
    REQUIRE(rj.get() == j);
    REQUIRE(pj.refcount() == 3);
    REQUIRE(qj.refcount() == 3);
    REQUIRE(rj.refcount() == 3);

    pi = pj;

    REQUIRE(pi.get() == j);
    REQUIRE(qi.get() == i);
    REQUIRE(ri.get() == i);
    REQUIRE(pi.refcount() == 4);
    REQUIRE(qi.refcount() == 2);
    REQUIRE(ri.refcount() == 2);

    REQUIRE(pj.get() == j);
    REQUIRE(qj.get() == j);
    REQUIRE(rj.get() == j);
    REQUIRE(pj.refcount() == 4);
    REQUIRE(qj.refcount() == 4);
    REQUIRE(rj.refcount() == 4);
}