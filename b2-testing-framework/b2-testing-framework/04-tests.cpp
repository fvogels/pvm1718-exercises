#include "Catch.h"
#include "test.h"
#include "test-suite.h"

/*

    1. Write a abstract class named TestSuite. It is a subtype of Test.
       It keeps a list of children (tests) and allows tests to be added to this list
       using an add method. This class does not implement run().

    2. Write a class RunAllTestSuite, subtype of TestSuite.
       Its run() method should iterate over all child tests and add the resulting scores
       together. The result is the sum of the scores of all child tests.

*/


namespace
{
    class DummyTest : public Test
    {
        Score score;

    public:
        DummyTest(const Score& score) : score(score) { }

        Score run() const override
        {
            return score;
        }
    };


    std::shared_ptr<Test> dummy(const Score& score)
    {
        return std::make_shared<DummyTest>(score);
    }
}

TEST_CASE("Creating RunAllTestSuite with one test")
{
    RunAllTestSuite suite;
    suite.add(dummy(Score(1, 1)));

    CHECK(suite.run() == Score(1, 1));
}

TEST_CASE("Creating RunAllTestSuite with two passing tests")
{
    RunAllTestSuite suite;
    suite.add(dummy(Score(1, 1)));
    suite.add(dummy(Score(1, 1)));

    CHECK(suite.run() == Score(2, 2));
}

TEST_CASE("Creating RunAllTestSuite with two failing tests")
{
    RunAllTestSuite suite;
    suite.add(dummy(Score(0, 1)));
    suite.add(dummy(Score(0, 1)));

    CHECK(suite.run() == Score(0, 2));
}

TEST_CASE("Creating RunAllTestSuite with three tests")
{
    RunAllTestSuite suite;
    suite.add(dummy(Score(0, 1)));
    suite.add(dummy(Score(1, 2)));
    suite.add(dummy(Score(2, 3)));

    CHECK(suite.run() == Score(3, 6));
}