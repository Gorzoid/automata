#include <gtest/gtest.h>
#include <automata.hpp>
#include <cstdio>

TEST(DFSA, SimpleConstruction1)
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


TEST(DFSA, SimpleConstruction2)
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

TEST(DFSA, SimpleSequence1)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("abc");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_FALSE(machine.match("aabc"));
    EXPECT_FALSE(machine.match("abcd"));
    EXPECT_FALSE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ab"));
}

TEST(DFSA, SimpleSequence2)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("a");

    EXPECT_TRUE(machine.match("a"));
    EXPECT_FALSE(machine.match("aa"));
    EXPECT_FALSE(machine.match(""));
    EXPECT_FALSE(machine.match("ab"));
    EXPECT_FALSE(machine.match("b"));
}

TEST(DFSA, SimpleKleeneStar)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("a");
    machine.plus();

    EXPECT_TRUE(machine.match("a"));
    EXPECT_TRUE(machine.match("aa"));
    EXPECT_TRUE(machine.match("aaa"));
    EXPECT_FALSE(machine.match(""));
    EXPECT_FALSE(machine.match("ab"));
    EXPECT_FALSE(machine.match("ba"));
    EXPECT_FALSE(machine.match("aab"));
}

TEST(DFSA, SimpleKleenePlus)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("a");
    machine.star();

    EXPECT_TRUE(machine.match("a"));
    EXPECT_TRUE(machine.match("aa"));
    EXPECT_TRUE(machine.match("aaa"));
    EXPECT_TRUE(machine.match(""));
    EXPECT_FALSE(machine.match("ab"));
    EXPECT_FALSE(machine.match("ba"));
    EXPECT_FALSE(machine.match("aab"));
}

TEST(DFSA, SequenceKleeneStar)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("abc");
    machine.plus();

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("abcabc"));
    EXPECT_TRUE(machine.match("abcabcabc"));
    EXPECT_FALSE(machine.match(""));
    EXPECT_FALSE(machine.match("ab"));
    EXPECT_FALSE(machine.match("ba"));
    EXPECT_FALSE(machine.match("aab"));
}

TEST(DFSA, SequenceKleenePlus)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("abc");
    machine.star();

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("abcabc"));
    EXPECT_TRUE(machine.match("abcabcabc"));
    EXPECT_TRUE(machine.match(""));
    EXPECT_FALSE(machine.match("abcab"));
    EXPECT_FALSE(machine.match("bcabc"));
    EXPECT_FALSE(machine.match("abcacc"));
}

TEST(DFSA, CombineRegex)
{
    using namespace automata;
    dfsa machine = dfsa::sequence("a");
    machine.star(); // a*

    machine += dfsa::sequence("b"); // a*b

    EXPECT_TRUE(machine.match("ab"));
    EXPECT_TRUE(machine.match("aab"));
    EXPECT_TRUE(machine.match("b"));
    EXPECT_FALSE(machine.match(""));
    EXPECT_FALSE(machine.match("bb"));
    EXPECT_FALSE(machine.match("abb"));
    EXPECT_FALSE(machine.match("aac"));
    EXPECT_TRUE(machine.match("aaab"));
}
/*
TEST(DFSA, CombineRegex2)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab") + dfsa::compile_regex("c*");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("ab"));
    EXPECT_TRUE(machine.match("abcc"));
    EXPECT_FALSE(machine.match("ac"));
    EXPECT_FALSE(machine.match("bc"));
    EXPECT_FALSE(machine.match("c"));
}

TEST(DFSA, CombineRegex3)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab*") + dfsa::compile_regex("c");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("ac"));
    EXPECT_TRUE(machine.match("abbc"));
    EXPECT_FALSE(machine.match("ab"));
    EXPECT_FALSE(machine.match("bc"));
    EXPECT_FALSE(machine.match("bb"));
}

TEST(DFSA, CombineRegex4)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab*") + dfsa::compile_regex("c*");

    EXPECT_TRUE(machine.match("abc"));
    EXPECT_TRUE(machine.match("a"));
    EXPECT_TRUE(machine.match("abb"));
    EXPECT_TRUE(machine.match("acc"));
    EXPECT_TRUE(machine.match("abbcc"));
    EXPECT_FALSE(machine.match("acb"));
    EXPECT_FALSE(machine.match("bc"));
}

TEST(DFSA, CombineIdentity1)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab");

    dfsa machine2 = machine + dfsa::compile_regex("");
    dfsa machine3 = dfsa::compile_regex("") + machine;

    EXPECT_EQ(machine.match("ab"),  machine2.match("ab")); // TRUE
    EXPECT_EQ(machine.match("ab"),  machine3.match("ab")); // TRUE

    EXPECT_EQ(machine.match("abc"), machine2.match("abc")); // FALSE
    EXPECT_EQ(machine.match("abc"), machine3.match("abc")); // FALSE

    EXPECT_EQ(machine.match("a"),   machine2.match("a")); // FALSE
    EXPECT_EQ(machine.match("a"),   machine3.match("a")); // FALSE

}

TEST(DFSA, CombineIdentity2)
{
    using namespace automata;
    dfsa machine = dfsa::compile_regex("ab*");

    dfsa machine2 = machine + dfsa::compile_regex("");
    dfsa machine3 = dfsa::compile_regex("") + machine;

    EXPECT_EQ(machine.match("ab"),  machine2.match("ab")); // TRUE
    EXPECT_EQ(machine.match("ab"),  machine3.match("ab")); // TRUE

    EXPECT_EQ(machine.match("abc"), machine2.match("abc")); // FALSE
    EXPECT_EQ(machine.match("abc"), machine3.match("abc")); // FALSE

    EXPECT_EQ(machine.match("a"),   machine2.match("a")); // TRUE
    EXPECT_EQ(machine.match("a"),   machine3.match("a")); // TRUE

    EXPECT_EQ(machine.match("abb"),   machine2.match("abb")); // TRUE
    EXPECT_EQ(machine.match("abb"),   machine3.match("abb")); // TRUE
}*/

/* Explicit main function to allow for quicker debugging */

/*int main(int argc, char **argv)
{
	  printf("Running main() from test.cpp\n");
	  testing::InitGoogleTest(&argc, argv);
	  return RUN_ALL_TESTS();
}*/
