#include <gtest/gtest.h>
#include <automata.hpp>

TEST(DFSA, SimpleCyclicMatch)
{
    using namespace automata;
    dfsa machine;
    machine.addNode(dfsa::node{
        {
            {'a', 0}, // cyclic
        },
        true
    });

    EXPECT_TRUE(machine.match("a"));
    EXPECT_TRUE(machine.match("aaaa"));
    EXPECT_FALSE(machine.match("baaa"));
    EXPECT_FALSE(machine.match("aaab"));
}