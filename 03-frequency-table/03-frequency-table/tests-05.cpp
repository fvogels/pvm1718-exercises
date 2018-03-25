#include "Catch.h"
#include "frequency-table.h"
#include <vector>
#include "test-aux.h"


TEST_CASE("{1, 1} <= {2, 2}")
{
    const FrequencyTable a = FREQTABLE(1, 1);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(a <= b);
}

TEST_CASE("{2, 2} <= {2, 2}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(a <= b);
}

TEST_CASE("{2, 2} <= {2, 3}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 3);

    CHECK(a <= b);
}

TEST_CASE("!({2, 3} <= {2, 2})")
{
    const FrequencyTable a = FREQTABLE(2, 3);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(!(a <= b));
}

TEST_CASE("!({3, 3} <= {2, 2})")
{
    const FrequencyTable a = FREQTABLE(3, 3);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(!(a <= b));
}

TEST_CASE("{2, 2} >= {1, 1}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(1, 1);

    CHECK(a >= b);
}

TEST_CASE("{2, 2} >= {2, 2}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(a >= b);
}

TEST_CASE("{2, 2} >= {2, 1}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 1);

    CHECK(a >= b);
}

TEST_CASE("!({1, 1} >= {2, 2})")
{
    const FrequencyTable a = FREQTABLE(1, 1);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(!(a >= b));
}

TEST_CASE("!({2, 2} >= {1, 3})")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(1, 3);

    CHECK(!(a >= b));
}

TEST_CASE("{2, 2} == {2, 2}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 2);

    CHECK(a == b);
}

TEST_CASE("!({2, 2} == {2, 3})")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 3);

    CHECK(!(a == b));
}

TEST_CASE("!({2, 2} == {3, 2})")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(3, 2);

    CHECK(!(a == b));
}

TEST_CASE("{2, 2} != {2, 3}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(2, 3);

    CHECK(a != b);
}

TEST_CASE("{2, 2} != {3, 2}")
{
    const FrequencyTable a = FREQTABLE(2, 2);
    const FrequencyTable b = FREQTABLE(3, 2);

    CHECK(a != b);
}

TEST_CASE("!({4, 4} != {4, 4})")
{
    const FrequencyTable a = FREQTABLE(4, 4);
    const FrequencyTable b = FREQTABLE(4, 4);

    CHECK(!(a != b));
}
