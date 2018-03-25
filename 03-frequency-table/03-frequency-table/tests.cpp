#define CATCH_CONFIG_MAIN
#include "Catch.h"
#include "frequency-table.h"


/*
    Create a class FrequencyTable.

    A FrequencyTable can be used to keep track of the frequencies of unsigned integers.
    See example-usage.cpp to see basic usage.

    A FrequencyTable must be initialized with a size N. It can then be used to keep counts
    of all unsigned integers from 0 to N-1.
    If no N is given, a default value of 10 is chosen.
    Important: you are only allowed to define ONE constructor.

    Internally, a FrequencyTable must make use of an array of unsigned integers,
    which must be allocated on the heap (you don't have the choice since
    you don't know at compile time how large this array needs to be).
    You are NOT allowed to define a destructor, yet you must not have memory leaks.
    In other words, make use of a smart pointer. Pick the one that makes the most sense in this context:
    you know that every FrequencyTable must have its very own array.

    The following operators must be defined:
        +, +=, <=, >=, ==, !=, =

    It is important that you write the fewest amount of algorithmic code as possible.
    Before you implement the operators, think of how you can express one operator
    in terms of others.

    ft1 <= ft2  iff  each frequency in ft1 is <= its corresponding frequency in ft2.
*/
