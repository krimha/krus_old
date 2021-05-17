#include <set>
#include <krus/nfa.h>
#include <krus/dfa.h>
#include <krus/utils.h>
#include <krus/state_wrapper.h>

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton(
	StateSet states,
	Alphabet alphabet,
	TransitionFunction f,
	State start_state,
	StateSet accept_states)
    : states_{states} 
    , alphabet_{alphabet} 
    , transition_function_{f} 
    , start_state_{start_state} 
    , accept_states_{accept_states} 
    , empty_string_{'\0'}
{

    // Split up transition function into eps and no eps
    for (const auto& [k, out] : transition_function_) {
	std::string in = k.first;
	char sym = k.second;

	if (sym != empty_string_) {
	    transition_function_no_eps_.insert({k, out});
	} else {
	    // TODO: Should do DFS to fully populate this
	    
	    auto out_set = out;
	    out_set.push_back(in);
	    E.insert({in, out_set});
	}

    } 
    
};

DeterministicFiniteAutomaton NondeterministicFiniteAutomaton::asDFA()
{

    // We need random access
    // TODO: is there a better way to do this?
    std::vector<State> states;
    for (const auto& state : getStates()) {
	states.push_back(state);
    }
    

    std::vector<StateWrapper> power_set;
    for (int intmap=0; intmap < (1 << getStates().size()); ++intmap) {
	int current_map = intmap;
	int current_index = 0;
	StateWrapper current_set;

	while (current_map != 0) {
	    if (current_map & 1) {
		current_set.insert(states[current_index]);
	    }
	    current_map >>= 1;
	    current_index++;
	}
	/* std::sort(current_set.begin(), current_set.end()); */
	power_set.push_back(current_set);
    }


    // Construct most basic transition function
    DeterministicFiniteAutomaton::TransitionFunction transition_function;
    for (const auto& state : power_set) {
	for (const auto& symbol : getAlphabet()) {
	    auto state_str = state.str();
	    transition_function.insert({{state_str, symbol}, {state_str}});
	}
    }

    // Find accept state. i.e. all subsets that contain an accept state of the NFA
    std::vector<StateWrapper> accept_states;
    for (const auto& subset : power_set) {
	for (const auto& accept_state : getAcceptStates()) {
	    if (subset.contains(accept_state)) {
		accept_states.emplace_back(subset);
		break; // only need to find one in each.
	    }
	}
    }

    /* std::vector<State> start_state{getStartState()}; */
    /* for (const auto& state : E[getStartState()]) { */
	/* start_state.push_back(state); */
    /* } */
    /* std::sort(start_state.begin(), start_state.end()); */
    /* auto start_state_str = iter_str(start_state); */
    /* std::cout << "start state is " << start_state_str << '\n'; */


    /* return DeterministicFiniteAutomaton { */
	    /* power_set_as_strings, */
	    /* getAlphabet(), */
	    /* f, */
	    /* start_state_str, */	    
	    /* accept_states */
    /* }; */

    std::vector<std::string> power_set_as_strings;
    for (const auto& state : power_set ) {
	power_set_as_strings.emplace_back(state.str());
    }

    std::vector<std::string> accept_states_as_strings;
    for (const auto& state : accept_states ) {
	accept_states_as_strings.emplace_back(state.str());
    }


    return DeterministicFiniteAutomaton {
	power_set_as_strings,
	getAlphabet(),
	{},
	"",
	{},
    };
}
