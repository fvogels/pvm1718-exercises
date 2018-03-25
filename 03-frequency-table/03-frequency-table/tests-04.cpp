#include "Catch.h"
#include "frequency-table.h"


TEST_CASE("Adding (+=) frequency tables should add each frequency separately")
{
    FrequencyTable a(5), b(5);

    a[0] = 5;
    a[1] = 2;

    b[0] = 3;
    b[2] = 7;

    a += b;

    CHECK(a[0] == 8);
    CHECK(a[1] == 2);
    CHECK(a[2] == 7);
    CHECK(a[3] == 0);
    CHECK(a[4] == 0);

    CHECK(b[0] == 3);
    CHECK(b[1] == 0);
    CHECK(b[2] == 7);
    CHECK(b[3] == 0);
    CHECK(b[4] == 0);
}

TEST_CASE("Const correctness of +=")
{
    FrequencyTable a(5), b(5);
    const FrequencyTable& c = b;

    a += c;
}

TEST_CASE("Adding frequency tables with +")
{
    FrequencyTable a(5), b(5);

    a[0] = 1;
    a[1] = 8;

    b[0] = 4;
    b[2] = 2;

    FrequencyTable c = a + b;

    CHECK(c[0] == 5);
    CHECK(c[1] == 8);
    CHECK(c[2] == 2);
    CHECK(c[3] == 0);
    CHECK(c[4] == 0);

    CHECK(a[0] == 1);
    CHECK(a[1] == 8);
    CHECK(a[2] == 0);
    CHECK(a[3] == 0);
    CHECK(a[4] == 0);

    CHECK(b[0] == 4);
    CHECK(b[1] == 0);
    CHECK(b[2] == 2);
    CHECK(b[3] == 0);
    CHECK(b[4] == 0);
}

TEST_CASE("Const correctness of +")
{
    const FrequencyTable a(5), b(5);
    FrequencyTable c = a + b;
}

TEST_CASE("+= should return reference to left operand")
{
    FrequencyTable a, b;
    FrequencyTable& c = a += b;

    CHECK(&c == &a);
}

TEST_CASE("+ should return new object")
{
    FrequencyTable a, b;
    FrequencyTable c = a + b;

    CHECK(&a != &b);
    CHECK(&a != &c);
    CHECK(&b != &c);
}

TEST_CASE("+'s result should store its contents in separate memory")
{
    FrequencyTable a, b;
    FrequencyTable c = a + b;

    CHECK(&a[0] != &c[0]);
    CHECK(&b[0] != &c[0]);
}
