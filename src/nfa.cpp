#include <krus/nfa.h>
#include <krus/dfa.h>

DeterministicFiniteAutomaton NondeterministicFiniteAutomaton::asDFA()
{

    // We need random access
    std::vector<State> states;
    for (const auto& state : getStates()) {
	states.push_back(state);
    }
    

    std::vector<std::vector<State>> power_set;
    for (int intmap=0; intmap < (1 << getStates().size()); ++intmap) {
	int current_map = intmap;
	int current_index = 0;
	StateSet current_set;

	while (current_map != 0) {
	    if (current_map & 1) {
		current_set.push_back(states[current_index]);
	    }
	    current_map >>= 1;
	    current_index++;
	}
	power_set.push_back(current_set);
    }


    std::vector<std::string> power_set_as_strings;
    for (const auto& set : power_set) {
	std::stringstream ss;
	ss << "{ ";
	for (const auto& x : set) {
	    ss << x << ' ';
	}	
	ss << '}';
	power_set_as_strings.emplace_back(ss.str());
    }


    return DeterministicFiniteAutomaton {
	    power_set_as_strings,
	    {},
	    {},
	    {"q1"},
	    {}
    };
}
