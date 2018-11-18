#include <automata.hpp>
namespace automata
{

    bool dfsa::match(const std::string& str) const
    {
        int state = m_initialState;
        for(char c : str)
        {
            auto it = m_transitions.find({state,c}); // Get next node given action
            if(it == m_transitions.end())
                return false;
            state = it->second;
        };

        return m_finalStates.count(state);
    }

    dfsa dfsa::compile_regex(const std::string& pattern)
    {
        dfsa machine;
        int currentState = 0;
        char lastChar;
        for(char c : pattern)
        {
            if(c == '+' && currentState != 0)
            {
                machine.addTransition(currentState, lastChar, currentState); // Let this state loop back to itself if given lastChar again
            }
            else if(c == '*' && currentState != 0)
            {
                currentState--;
                machine.removeTransition(currentState, lastChar); // Remove last transition because lastChar just leads back to itself.
                machine.addTransition(currentState, lastChar, currentState);
            }
            else
            {
                machine.addTransition(currentState, c, currentState+1);
                currentState++;
            }
            lastChar = c;
        }

        machine.addFinalState(currentState);

        return machine;
    }

}