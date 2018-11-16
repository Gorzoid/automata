#include <gtest/gtest.h>
#include <automata.hpp>
#include <cstdio>

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
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleCompileRegex)
{
    using namespace automata;
    dfsa machine{"abc"};

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("aabc"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleRepeatRegex)
{
    using namespace automata;
    dfsa machine{"ab*c"};

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("ac"));
    EXPECT_TRUE(machine.match("abbbbc"));
    EXPECT_FALSE(machine.match("acbbb"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

int main(int argc, char **argv)
{
	  printf("Running main() from gtest_main.cc\n");
	  testing::InitGoogleTest(&argc, argv);
	  return RUN_ALL_TESTS();
}