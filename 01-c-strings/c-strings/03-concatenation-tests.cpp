#include "Catch.h"
#include "student.h"

/*
    Write a a function concatenate that takes two C-style strings and returns a new string (heap allocated)
    that is equal to the concatenation to the two given strings. For example, concatenate("abc", "xyz") should return "abcxyz".
*/

#define TEST_CONCATENATE(string1, string2, expected)  TEST_CASE("\"" string1 "\" + \"" string2 "\" -> \"" expected "\"") { check_concatenate(string1, string2, expected); }

void check_concatenate(char* s1, char* s2, char* expected)
{
    char* actual = concatenate(s1, s2);

    REQUIRE(string_length(s1) + string_length(s2) == string_length(actual));
    REQUIRE(equal_strings(actual, expected));

    delete[] actual;
}

TEST_CONCATENATE("", "", "")
TEST_CONCATENATE("a", "", "a")
TEST_CONCATENATE("", "b", "b")
TEST_CONCATENATE("a", "b", "ab")
TEST_CONCATENATE("abc", "def", "abcdef")
TEST_CONCATENATE("abc", "123456", "abc123456")
TEST_CONCATENATE("aaaa", "bbbb", "aaaabbbb")
TEST_CONCATENATE("aaaa", "aaaa", "aaaaaaaa")
TEST_CONCATENATE("aaaa", "a", "aaaaa")
TEST_CONCATENATE("a", "aaaa", "aaaaa")
