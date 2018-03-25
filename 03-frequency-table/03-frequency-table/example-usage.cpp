#include "Catch.h"
#include "frequency-table.h"


TEST_CASE("Counting the frequencies of the elements of a vector")
{
    std::vector<unsigned> ns = { 0, 1, 4, 3, 6, 4, 3, 4, 1, 6, 4, 3, 1 };
    FrequencyTable table;

    // Loop over ns' items
    for (auto n : ns)
    {
        // Increment count of each n
        table[n]++;
    }

    // 0 appears once in ns
    CHECK(table[0] == 1);

    // 1 appears thrice in ns
    CHECK(table[1] == 3);

    // 2 does not appear in ns
    CHECK(table[2] == 0);
}
