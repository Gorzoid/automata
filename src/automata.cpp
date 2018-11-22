#include <automata.hpp>
#include <functional>
namespace automata
{

    void dfsa::addTransition(int fromState, char c, int toState)
    {
        m_transitions[{fromState, c}] = toState;
        m_maxStates = std::max(m_maxStates, std::max(fromState,toState));
    }
    void dfsa::removeTransition(int fromState, char c)
    {
        m_transitions.erase({fromState, c});
    }
    void dfsa::addFinalState(int state)
    {
        m_finalStates.insert(state);
    }

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

        return m_finalStates.count(state)==1;
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

    dfsa& dfsa::operator+=(const dfsa& machine)
    {
        using namespace std::placeholders;
        int s = machine.m_initialState;
        for(std::pair<std::pair<int,char>,int> p : machine.m_transitions) // explicit type instead of auto to remove const'ness
        {
            p.second += m_maxStates; // Translate state index to avoid clashing with our own state index
            p.first.first += m_maxStates;
            if(p.first.first == m_maxStates + machine.m_initialState)
            {
                // if this is transition for initial state, attach to all our final states instead
                for(int f : m_finalStates)
                {
                    m_transitions[{f,p.first.second}] = p.second;
                }
            }
            else
            {
                m_transitions[{p.first.first,p.first.second}] = p.second;
            }
        }
        
        m_finalStates.clear();
        std::transform(
            machine.m_finalStates.begin(),
            machine.m_finalStates.end(),
            std::inserter(m_finalStates, m_finalStates.end()),
            std::bind(std::plus<int>{}, m_maxStates, _1));

        m_maxStates += machine.m_maxStates;
        return *this;
    }

    dfsa operator+(dfsa machine1, const dfsa& machine2)
    {
        machine1 += machine2;
        return machine1;
    }

}
