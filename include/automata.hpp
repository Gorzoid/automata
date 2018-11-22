#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace automata
{
    class dfsa
    {
        struct pair_hasher
        {
            template<typename T1, typename T2>
            std::size_t operator()(const std::pair<T1,T2>& p) const
            {
                std::size_t hash = std::hash<T1>{}(p.first);
                return hash ^ (std::hash<T2>{}(p.second) + 0x9e3779b9 + (hash<<6) + (hash>>2));
            }
        };

    private:
        std::unordered_map<std::pair<int,char>,int,pair_hasher> m_transitions;
        std::unordered_set<int> m_finalStates;
        int m_initialState = 0;
        int m_maxStates = 0;

    public:
        dfsa() = default;

        void addTransition(int fromState, char c, int toState);
        void removeTransition(int fromState, char c);
        void addFinalState(int state);

        bool match(const std::string& input) const;
        
        dfsa& operator+=(const dfsa& machine);

        friend dfsa operator+(dfsa machine1, const dfsa& machine2);

        static dfsa compile_regex(const std::string& pattern);
    };
}
