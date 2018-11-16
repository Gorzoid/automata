#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

namespace automata
{
    class dfsa
    {
    public: // temporary public everything so we can test smaller features.
        struct node
        {
            std::unordered_map<char, int> mapping; // mapping from char to index in dfsa::nodes
            bool finalState;
            node(std::unordered_map<char, int> _mapping, bool _finalState = false)
                : mapping(std::move(_mapping)), finalState(_finalState)
            {

            }

            int transition(char c) const
            {
                auto it = mapping.find(c);
                if(it == mapping.end()) return -1;
                return it->second;
            }
        };

    //private:
        std::vector<node> nodes;
    public:
        dfsa(const std::string& regular_expression)
        {
            for(char c : regular_expression)
            {
                nodes.emplace_back(std::unordered_map<char,int>{{c, nodes.size()+1}},false);
            }
            nodes.emplace_back(std::unordered_map<char,int>{},true);
        }
        dfsa() = default;

        void addNode(node newNode)
        {
            nodes.push_back(std::move(newNode));
        }

        bool match(const std::string& str)
        {
            if(nodes.empty()) return false; // Empty state machine never matches

            int state = 0;
            for(char c : str)
            {
                int nextState = nodes[state].transition(c); // Get next node given action
                if(nextState >= nodes.size() || nextState < 0)
                    return false;
                state = nextState;
            };

            return nodes[state].finalState;
        }
    };
}