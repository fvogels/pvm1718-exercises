#include "settings.h"

#ifdef TEST_BUILD

#include "midi.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Reading id from header with id MThd")
{
    CHUNK_HEADER header{ { 'M', 'T', 'h', 'd'}, 0 };
    std::string id = header_id(header);

    CHECK(id == "MThd");
}

TEST_CASE("Reading id from header with id MTrk")
{
    CHUNK_HEADER header{ { 'M', 'T', 'r', 'k' }, 111 };
    std::string id = header_id(header);

    CHECK(id == "MTrk");
}

TEST_CASE("Reading id from header with id abcd")
{
    CHUNK_HEADER header{ { 'a', 'b', 'c', 'd' }, 78651 };
    std::string id = header_id(header);

    CHECK(id == "abcd");
}

#endif
