#include "Catch.h"
#include "my_shared_ptr.h"
#include "util.h"

/*
    Define the destructor. If there is a pointee (i.e., it is not nullptr),
    the refcount should be decreased by one. If the refcount reaches zero,
    both the pointee and the refcount should be freed.
*/


TEST_CASE("Destructor deletes pointee")
{
    int destructions = 0;
    Helper* h = new Helper([]() {}, [&destructions]() { destructions++; });

    {
        my_shared_ptr<Helper> p(h);
    }

    CHECK(destructions == 1);
}
