#include "Catch.h"
#include "test.h"
#include "test-suite.h"

/*

    Write a class FailFastTestSuite. Like FailFastTestSuite, it subclasses TestSuite.

    While FailFastTestSuite runs all tests, FailFastTestSuite stops running tests
    as soon as one test fails (= does not yield a perfect score).

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

TEST_CASE("Creating FailFastTestSuite with one test")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(1, 1)));

    CHECK(suite.run() == Score(1, 1));
}

TEST_CASE("Creating FailFastTestSuite with two passing tests")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(1, 1)));
    suite.add(dummy(Score(1, 1)));

    CHECK(suite.run() == Score(2, 2));
}

TEST_CASE("Creating FailFastTestSuite with two failing tests")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(0, 1)));
    suite.add(dummy(Score(0, 1)));

    CHECK(suite.run() == Score(0, 1));
}

TEST_CASE("Creating FailFastTestSuite with three tests, first fails")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(0, 1)));
    suite.add(dummy(Score(1, 2)));
    suite.add(dummy(Score(2, 3)));

    CHECK(suite.run() == Score(0, 1));
}

TEST_CASE("Creating FailFastTestSuite with three tests, first passes, second fails")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(1, 1)));
    suite.add(dummy(Score(1, 2)));
    suite.add(dummy(Score(2, 3)));

    CHECK(suite.run() == Score(2, 3));
}

TEST_CASE("Creating FailFastTestSuite with three tests, first & second pass, third fails")
{
    FailFastTestSuite suite;
    suite.add(dummy(Score(1, 1)));
    suite.add(dummy(Score(2, 2)));
    suite.add(dummy(Score(2, 3)));

    CHECK(suite.run() == Score(5, 6));
}