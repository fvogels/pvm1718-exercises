#include "Catch.h"
#include "frequency-table.h"


TEST_CASE("Fresh table should have all frequencies set to 0")
{
    FrequencyTable ft;

    for (unsigned i = 0; i != 10; ++i)
    {
        CHECK(ft[i] == 0);
    }
}

TEST_CASE("Checking const correctness of indexing")
{
    const FrequencyTable ft;

    for (unsigned i = 0; i != 10; ++i)
    {
        CHECK(ft[i] == 0);
    }
}

TEST_CASE("Indexing should allow changes to be made on non-const tables")
{
    FrequencyTable ft;

    for (unsigned i = 0; i != 10; ++i)
    {
        ft[i] += i;
    }

    for (unsigned i = 0; i != 10; ++i)
    {
        CHECK(ft[i] == i);
    }
}