#include "Catch.h"
#include "frequency-table.h"


TEST_CASE("Creating table of size 5; size() should return 5")
{
    FrequencyTable ft(5);

    CHECK(ft.size() == 5);
}

TEST_CASE("Creating table of default size; size() should return 10")
{
    FrequencyTable ft;

    CHECK(ft.size() == 10);
}

TEST_CASE("Checking const correctness of size()")
{
    const FrequencyTable ft;

    CHECK(ft.size() == 10);
}
