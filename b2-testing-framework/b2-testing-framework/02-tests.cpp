#include "test.h"


/*
    Define an abstract superclass Test. Deduce its members from the code below.
    This file does not contain any tests, it merely has to compile.
*/


namespace // <-- You can ignore this line
{
    class DummyTest : public Test
    {
    public:
        Score run() const override
        {
            return Score();
        }
    };
}