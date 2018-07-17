#define CATCH_CONFIG_MAIN
#include "Catch.h"

/*

    For this exercise, you will be making a small testing framework.
    Imagine it being used as a way to evaluate a student's knowledge.

    The central class is Test, which has only one method: run.
    The run-method returns a Score object, which represents how the test went.

    A test can exhibit a hierarchical structure. For example,


        [test suite]  ------ [test suite about X] ------------ [X: Test 1]
             |                                              |
             |                                              |- [X: Test 2]
             |                                              |
             |                                              |- [X: Test 3]
             |
             |-------------- [test suite about Y] ------------ [Y: Test 1]
                                                            |
                                                            |- [Y: Test 2]


    You can compare it to a file system: a "test suite" corresponds to a directory,
    and "regular" tests are like files. In other words, a suite groups multiple tests
    together and makes them act like one.

*/
