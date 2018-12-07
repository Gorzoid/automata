#include <automata.hpp>
#include <functional>
#include <iterator>
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

    void dfsa::plus()
    {
        /// TODO:
        // This intially seems simple enough. Just attach finals states back to the initial state
        // However we face a problem if the final state and initial state share a transition symbol
        // For now we will have to ignore this, but we will need to make a function that basically
        // propagates through a DFSA to resolve any of these conflicts

        // No easy way to find transitions when we only have fromState, so loop through each one.
        for(auto p : m_transitions)
        {
            if(p.first.first == m_initialState)
            {
                for(auto f : m_finalStates)
                    addTransition(f, p.first.second, p.second); // Can now loop back to restart state. Final states remain the same
            }
        }
    }

    void dfsa::star()
    {
        // Naive solution is to set m_initialState to a final state and call plus()
        // However if there are non final transitions to m_initialState
        // then this would cause undesired behaviour
        // This means we need to make a new initial state and mimic the original one

        // TODO:
        // We also face problems similar to the one addressed in dfsa::plus()

        int st = m_maxStates++;
        for(auto p : m_transitions)
        {
            if(p.first.first == m_initialState)
            {
                addTransition(st, p.first.second, p.second);
            }
        }

        m_initialState = st;

        plus(); // Avoid repeating code
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

    dfsa dfsa::sequence(const std::string& str)
    {
        dfsa machine;
        for(int i = 0; i < str.size(); ++i)
        {
            machine.addTransition(i, str[i], i+1);
        }
        machine.addFinalState(str.size());
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
            m_transitions[{p.first.first,p.first.second}] = p.second;
        }
        
        if(machine.m_finalStates.count(s) == 0) // Keep original contents only if initial state is also final state 
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
        machine1 += machine2; // pass by copy so this doesn't affect pass argument
        return machine1;
    }

    dfsa& dfsa::operator|=(const dfsa& machine)
    {

    }

    dfsa operator|(dfsa machine1, const dfsa& machine2)
    {
        machine1 |= machine2; // pass by copy so this doesn't affect pass argument
        return machine1;
    }

}
