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


TEST(DFSA, SimpleSequenceMatch)
{
    using namespace automata;
    dfsa machine;
    machine.addNode(dfsa::node{
        {
            {'a', 1},
        }
    });
    machine.addNode(dfsa::node{
        {
            {'b', 2},
        }
    });
    machine.addNode(dfsa::node{
        {
            {'c', 3},
        }
    });
    machine.addNode(dfsa::node{{}, true});

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("aabc"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
}
