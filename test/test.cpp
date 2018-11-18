#include <gtest/gtest.h>
#include <automata.hpp>
#include <cstdio>

TEST(DFSA, SimpleCyclicMatch)
{
    using namespace automata;
    dfsa machine;
    machine.addTransition(0, 'a', 0);
    machine.addFinalState(0);

    EXPECT_TRUE(machine.match("a"));
    EXPECT_TRUE(machine.match("aaaa"));
    EXPECT_FALSE(machine.match("baaa"));
    EXPECT_FALSE(machine.match("aaab"));
}


TEST(DFSA, SimpleSequenceMatch)
{
    using namespace automata;
    dfsa machine;
    machine.addTransition(0, 'a', 1);
    machine.addTransition(1, 'b', 2);
    machine.addTransition(2, 'c', 3);
    
    machine.addFinalState(3);

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("aabc"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleCompileRegex)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("abc");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("aabc"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleRepeatStarRegex)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab*c");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("ac"));
    EXPECT_TRUE(machine.match("abbbbc"));
    EXPECT_FALSE(machine.match("acbbb"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleRepeatPlusRegex)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab+c");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("ac"));
    EXPECT_TRUE(machine.match("abbbbc"));
    EXPECT_FALSE(machine.match("acbbb"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

/* Explicit main function to allow for quicker debugging */

/*int main(int argc, char **argv)
{
	  printf("Running main() from test.cpp\n");
	  testing::InitGoogleTest(&argc, argv);
	  return RUN_ALL_TESTS();
}*/
