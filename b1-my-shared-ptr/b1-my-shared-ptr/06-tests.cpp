#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Overload the -> operator so as to be able to
    use p->member to access the pointee's members.
*/


struct Foo
{
    Foo() : x(0) { }

    int x;
};


TEST_CASE("Member access through single pointer")
{
    my_shared_ptr<Foo> p(new Foo);
    p->x++;
}

TEST_CASE("Member access through multiple pointers")
{
    my_shared_ptr<Foo> p(new Foo);
    my_shared_ptr<Foo> q(p);

    CHECK(p->x == 0);
    CHECK(q->x == 0);
    
    p->x++;
    
    CHECK(p->x == 1);
    CHECK(q->x == 1);

    q->x++;

    CHECK(p->x == 2);
    CHECK(q->x == 2);
}
