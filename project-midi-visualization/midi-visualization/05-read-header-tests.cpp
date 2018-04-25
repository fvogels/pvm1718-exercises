#include "settings.h"

#ifdef TEST_BUILD

#include "midi.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Checking that CHUNK_HEADER is defined correctly")
{
    static_assert(std::is_same<decltype(CHUNK_HEADER::id), char[4]>::value, "CHUNK_HEADER is lacking member 'id' of the appropriate type");
    static_assert(std::is_same<decltype(CHUNK_HEADER::size), uint32_t>::value, "CHUNK_HEADER is lacking member 'size' of appropriate type");
    static_assert(sizeof(CHUNK_HEADER) == sizeof(char[4]) + sizeof(uint32_t), "CHUNK_HEADER does not have the right size");

    static_assert(offsetof(CHUNK_HEADER, id) == 0, "CHUNK_HEADER's id field does not have the correct offset");
    static_assert(offsetof(CHUNK_HEADER, size) == 4, "CHUNK_HEADER's size field does not have the correct offset");
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'h', 'd', 0, 0, 0, 0 }")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0, 0, 0, 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    CHUNK_HEADER header;
    read_header(ss, &header);
    std::string type(header.id, sizeof(header.id));

    CHECK(type == "MThd");
    CHECK(header.size == 0);
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'h', 'd', 0x01, 0, 0, 0 }")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0x01, 0, 0, 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    CHUNK_HEADER header;

    REQUIRE(read_header(ss, &header));

    SECTION("Checking id")
    {
        std::string type(header.id, sizeof(header.id));
        CHECK(type == "MThd");
    }

    SECTION("Checking size")
    {
        INFO("Have you taken into account endianness?")
            CHECK(header.size == 0x01000000);
    }
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'r', 'k', 0x01, 0x02, 0x03, 0x04 }")
{
    char buffer[] = { 'M', 'T', 'r', 'k', 0x01, 0x02, 0x03, 0x04 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    CHUNK_HEADER header;

    REQUIRE(read_header(ss, &header));

    SECTION("Checking id")
    {
        std::string type(header.id, sizeof(header.id));
        CHECK(type == "MTrk");
    }

    SECTION("Checking size")
    {
        INFO("Have you taken into account endianness?")
            CHECK(header.size == 0x01020304);
    }
}

#endif
