#include <set>
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
	std::sort(current_set.begin(), current_set.end());
	power_set.push_back(current_set);
    }


    // Get string representation of power set elements
    DeterministicFiniteAutomaton::StateSet power_set_as_strings;

    for (const auto& set : power_set) {
	std::stringstream ss;
	ss << "{ ";
	for (const auto& x : set) {
	    ss << x << ' ';
	}	
	ss << '}';
	power_set_as_strings.emplace_back(ss.str());
    }

    // Construct most basic transition function
    DeterministicFiniteAutomaton::TransitionFunction f;

    for (size_t i=0; i < power_set.size(); ++i) {
	const auto& state = power_set[i];
	const auto& state_str = power_set_as_strings[i];

	for (const auto& symbol : getAlphabet()) {
	    std::set<State> all_candidates;

	    for (const auto& r : state) {
		for (const auto& s : transition_function_[std::make_pair(r,symbol)]) {
		    all_candidates.insert(s);
		}
	    }

	    std::vector<State> all_candidates_vec;
	    for (const auto& x : all_candidates) {
		all_candidates_vec.push_back(x);
	    }

	    std::sort(all_candidates_vec.begin(), all_candidates_vec.end());

	    std::stringstream ss;
	    ss << "{ ";
	    for (const auto& x : all_candidates_vec) {
		ss << x << ' ';
	    }	
	    ss << '}';
	    f.insert({{state_str, symbol}, ss.str()});
	}
    }

    // Find accept state
    DeterministicFiniteAutomaton::StateSet accept_states;
    for (const auto& set : power_set) {
	for (const auto& accept_state : getAcceptStates()) {
	    // The current subset contains an accept state for the original set
	    if (std::find(set.begin(), set.end(), accept_state) != set.end()) {
		// Assume that the set is sorted. Might potentially run into issues with e.g.
		// { q1, q2 } and { q2, q1 } not being equal.
		std::stringstream ss;
		ss << "{ ";
		for (const auto& x : set)
		    ss << x << ' ';
		ss << '}';
		accept_states.emplace_back(ss.str());
		break;
	    }
	}
    }

    return DeterministicFiniteAutomaton {
	    power_set_as_strings,
	    getAlphabet(),
	    f,
	    "{ " + getStartState() + " }",
	    accept_states
    };
}
