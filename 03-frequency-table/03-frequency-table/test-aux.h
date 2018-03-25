#ifndef TEST_AUX_H
#define TEST_AUX_H

#define FREQTABLE(...) create_table(std::vector<unsigned> { __VA_ARGS__ })

namespace
{
    FrequencyTable create_table(const std::vector<unsigned>& ns)
    {
        FrequencyTable ft(ns.size());

        for (unsigned i = 0; i != ns.size(); ++i)
        {
            ft[i] = ns[i];
        }

        return ft;
    }
}

#endif
