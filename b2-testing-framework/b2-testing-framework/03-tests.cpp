#include "Catch.h"
#include "test.h"
#include "scaler.h"

/*
    A Scaler is a Test subclass. It is parameterized in
        * A Test "child"
        * A double "maximum"

    A Scaler wraps around another Test and rescales its score.
    For example, say I have a test that returns a Score object on 1 (i.e., either 0/1 for failure or 1/1 for success).
    I consider this test to be important, so I want its outcome to have more effect on the total score. Instead
    of having a maximum of 1, I want it to be, say, 5. This way, this test is 5x more important than other tests.
    The Scaler's job is to rescale this score.


                    Scaler   ----wraps--->   Test
                     ?/5                      ?/1


    See the tests below for more use cases.
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
}

TEST_CASE("Scaling test returning 1/1 to 5/5")
{
    auto test = std::make_shared<DummyTest>(Score(1, 1)); // Tests that always returns 1/1
    Scaler scaler(test, 5);

    CHECK(scaler.run() == Score(5, 5));
}

TEST_CASE("Scaling test returning 0/1 to 0/4")
{
    auto test = std::make_shared<DummyTest>(Score(0, 1)); // Tests that always returns 0/1
    Scaler scaler(test, 4);

    CHECK(scaler.run() == Score(0, 4));
}