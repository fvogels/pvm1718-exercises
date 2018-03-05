#include "Catch.h"
#include "student.h"


/*
    Write a function equal_strings that takes two C-style strings and returns true if the given strings
    are equal, false otherwise.
*/

#define TEST_EQUAL(string)             TEST_CASE("\"" string "\" == \"" string "\"")   { check_string_equality(("a" string) + 1, ("b" string) + 1, true); }
#define TEST_UNEQUAL(string1, string2) TEST_CASE("\"" string1 "\" != \"" string2 "\"") { check_string_equality(string1, string2, false); }

void check_string_equality(const char* s1, const char* s2, bool expected)
{
    bool actual = equal_strings(const_cast<char*>(s1), const_cast<char*>(s2));

    CHECK(s1 != s2);
    REQUIRE(actual == expected);
}

TEST_EQUAL("")
TEST_EQUAL("a")
TEST_EQUAL("abc")
TEST_EQUAL("aaaaa")
TEST_EQUAL("4869156")

TEST_UNEQUAL("a", "b")
TEST_UNEQUAL("a", "aa")
TEST_UNEQUAL("", "x")
TEST_UNEQUAL("x", "")
TEST_UNEQUAL("xxx", "xxxx")
TEST_UNEQUAL("xxxxx", "xxxx")
