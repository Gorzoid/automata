#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace automata
{
    class dfsa
    {
    public: // temporary public everything so we can test smaller features.
        class node
        {
            std::unordered_map<char, int> m_mapping; // mapping from char to index in dfsa::nodes
            bool m_finalState;
        public:
            node(std::unordered_map<char, int> mapping, bool finalState = false)
                : m_mapping(std::move(mapping)), m_finalState(finalState)
            {

            }

            int transition(char c) const
            {
                auto it = m_mapping.find(c);
                if(it == m_mapping.end()) return -1;
                return it->second;
            }

            bool finalState() const
            {
                return m_finalState;
            }
        };

    //private:
        std::vector<node> nodes;
    public:
        dfsa(const std::string& regular_expression);
        dfsa() = default;

        void addNode(node newNode)
        {
            nodes.push_back(std::move(newNode));
        }

        bool match(const std::string& str)
        {
            int i = 0;
            return std::all_of(str.begin(), str.end(), [this,&i](char c) mutable {
                i = nodes[i].transition(c);
                return i >= 0;
            }) && nodes[i].finalState();
        }
    };
}