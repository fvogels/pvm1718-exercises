#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Overload the * operator (not multiplication, but dereferencing) so
    that we can write *p to reach the pointee.
*/


TEST_CASE("Dereferencing my_shared_ptr")
{
    int* i = new int(100);
    my_shared_ptr<int> p(i);

    CHECK(*p == *i);
}

TEST_CASE("Dereferencing const my_shared_ptr")
{
    int* i = new int(100);
    const my_shared_ptr<int> p(i);

    CHECK(*p == *i);
}
