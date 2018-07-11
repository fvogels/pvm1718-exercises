#include "Catch.h"
#include "my_shared_ptr.h"

/*
    Create a templated class my_shared_ptr.
    Determine which two fields it needs: one to store a link to the pointee, and one for the refcount.

    Define a default constructor. The default constructor should initialize the shared pointer to
    the equivalent of nullptr, i.e., there is no pointee, and hence no refcount.

    Define a method get() that allows to fetch a raw pointer to the pointee.
*/


TEST_CASE("Default constructor initializes pointer to nullptr")
{
    my_shared_ptr<int> p;

    CHECK(p.get() == nullptr);
}
