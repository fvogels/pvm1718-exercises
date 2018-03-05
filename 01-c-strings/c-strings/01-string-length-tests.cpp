#include "Catch.h"
#include "student.h"


/*
    In C, strings are represented using char*, i.e., a pointer to chars (~ array of chars).
    A char* only you where the string starts, but how would you determine how long the string is?
    C's solution is to terminate a string with 0.
    For example, the string "abc" takes 4 bytes of memory: 'a', 'b', 'c' and 0.

    Write a function string_length that takes a C-style string and returns how long it is.
    The 0 at the end is not counted. For example, the string "abc" has length 3 (even though it takes 4 bytes of memory.)

    Do NOT rely on library functions to solve these exercises.

    You will only be able to run the tests once everything compiles. To compile only one set of tests
    (e.g., those pertaining to string_length), you can exclude them by right-clicking on their respective
    files in the Solution Explorer and choose "Exclude From Project".
    To reincorporate them, first click "Show All Files" in the toolbar at the top of the Solution Explorer,
    then right click on the test file you wish to add, and pick "Include In Project".
*/


#define TEST_STRING_LENGTH(string, expected)  TEST_CASE("Length of \"" string "\"") { check_string_length(string, expected); }

void check_string_length(char* s, int expected)
{
    int actual = string_length(s);

    REQUIRE(expected == actual);
}

TEST_STRING_LENGTH("", 0)
TEST_STRING_LENGTH("a", 1)
TEST_STRING_LENGTH("abc", 3)
TEST_STRING_LENGTH("123456789", 9)
TEST_STRING_LENGTH("aaaaaaaaaaaaaaaa", 16)
