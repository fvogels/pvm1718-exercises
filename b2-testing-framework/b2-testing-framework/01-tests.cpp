#include "Catch.h"
#include "score.h"
#include <sstream>

/*
    Write a Score class. A score is a little bit like a fraction: "you score K out of N", i.e.
    the Score class has two fields of type double, which we'll name "value" and "maximum".

    Deduce from the tests what each method/operator should do.

    Be careful: addition and comparison are to be implemented differently than what you
                would do for fractions. Scores have a much simpler implementation.
                For example, 1/1 is different from 5/5 (if they behaved like true fractions, these would be equal).
*/


TEST_CASE("Score default constructor")
{
    Score score;

    CHECK(score.value == Approx(0));
    CHECK(score.maximum == Approx(0));
}

TEST_CASE("Score unary constructor")
{
    Score score(5);

    CHECK(score.value == Approx(0));
    CHECK(score.maximum == Approx(5));
}

TEST_CASE("Score binary constructor")
{
    Score score(1, 2);

    CHECK(score.value == Approx(1));
    CHECK(score.maximum == Approx(2));
}

/*
    Careful: this is not typical fraction addition, but much simpler than that.
*/
TEST_CASE("Score addition +")
{
    const Score a(1, 2);
    const Score b(4, 7);
    Score c = a + b;

    CHECK(c.value == Approx(1 + 4));
    CHECK(c.maximum == Approx(2 + 7));
}

TEST_CASE("Score addition +=")
{
    const Score a(5, 6);
    Score b(2, 3);

    b += a;

    CHECK(b.value == Approx(5 + 2));
    CHECK(b.maximum == Approx(6 + 3));
}

TEST_CASE("Comparing scores using ==")
{
    CHECK(Score(1, 1) == Score(1, 1));
    CHECK(Score(5, 5) == Score(5, 5));

    CHECK(!(Score(0, 1) == Score(1, 1)));
    CHECK(!(Score(1, 1) == Score(5, 5)));
}

TEST_CASE("Comparing scores using !=")
{
    CHECK(!(Score(1, 1) != Score(1, 1)));
    CHECK(!(Score(5, 5) != Score(5, 5)));

    CHECK(Score(0, 1) != Score(1, 1));
    CHECK(Score(1, 1) != Score(5, 5));
}

TEST_CASE("Scaling scores")
{
    const Score a(3, 6);
    Score b = a.scale(10);

    CHECK(b.value == Approx(5));
    CHECK(b.maximum == Approx(10));
}

TEST_CASE("Score success")
{
    CHECK(Score(1, 1).success());
    CHECK(Score(5, 5).success());
    CHECK(Score(10, 10).success());

    CHECK(!Score(0, 5).success());
    CHECK(!Score(0, 1).success());
    CHECK(!Score(3, 4).success());
}

TEST_CASE("Operator <<")
{
    std::stringstream sstream;

    sstream << Score(1, 4);

    CHECK(sstream.str() == "1/4");
}