#include "Catch.h"
#include "frequency-table.h"
#include <vector>
#include "test-aux.h"


TEST_CASE("Copy constructor")
{
    FrequencyTable a = FREQTABLE(5, 2, 5);
    FrequencyTable b(a);

    CHECK(a[0] == 5);
    CHECK(a[1] == 2);
    CHECK(a[2] == 5);

    CHECK(b[0] == 5);
    CHECK(b[1] == 2);
    CHECK(b[2] == 5);
}

TEST_CASE("Const correctness of copy constructor")
{
    const FrequencyTable a = FREQTABLE(5, 2, 5);
    FrequencyTable b(a);
}

TEST_CASE("Copy has its own memory")
{
    FrequencyTable a = FREQTABLE(5, 2, 5);
    FrequencyTable b(a);

    CHECK(&a[0] != &b[0]);
}

TEST_CASE("Assignment")
{
    FrequencyTable a = FREQTABLE(1, 2, 3, 4);
    FrequencyTable b = FREQTABLE(8, 7, 6, 5);

    a = b;

    CHECK(a[0] == b[0]);
    CHECK(a[1] == b[1]);
    CHECK(a[2] == b[2]);
    CHECK(a[3] == b[3]);
}

TEST_CASE("Const correctness of assignment")
{
    FrequencyTable a;
    const FrequencyTable b;

    a = b;
}

TEST_CASE("Assignment returns lhs")
{
    FrequencyTable a, b;
    FrequencyTable& c = (a = b);

    CHECK(&c == &a);
}

TEST_CASE("Assignment does not lead to shared memory")
{
    FrequencyTable a, b;

    a = b;

    CHECK(&a[0] != &b[0]);
}
