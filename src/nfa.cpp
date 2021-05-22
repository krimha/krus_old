#include <set>
#include <queue>
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

	    eps_reachable.insert({in, {}});
	    for (const auto& x : out_set) {
		eps_reachable[in].insert(in);
	    }
	}

    } 

    initialize_eps_reachable();
    
};

DeterministicFiniteAutomaton NondeterministicFiniteAutomaton::asDFA()
{

    // We need random access to compute the permutateions
    // TODO: is there a better way to do this?
    std::vector<State> states;
    for (const auto& state : getStates()) {
	states.push_back(state);
    }
    

    // Find set of states
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

    // Find start state
    StateWrapper start_state;
    for (const auto& state : eps_reachable[start_state_]) {
	start_state.insert(state);
    }
    

    // Construct most basic transition function
    DeterministicFiniteAutomaton::TransitionFunction transition_function;
    for (const auto& symbol : getAlphabet()) {
	for (const auto& dfa_src_state : power_set) {
	    StateWrapper dfa_target_state;

	    for (const auto& nfa_src_state : dfa_src_state.getSet()) {
		for (const auto& nfa_target_state : transition_function_[{nfa_src_state, symbol}]) {
		    dfa_target_state.insert(nfa_target_state);

		    for (const auto& eps_reachable_state : eps_reachable[nfa_target_state]) {
			dfa_target_state.insert(eps_reachable_state);
		    }
		}
	    }

	    transition_function.insert({{dfa_src_state.str(), symbol}, dfa_target_state.str()});
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

    // Convert to strings
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
	transition_function,
	start_state.str(),
	accept_states_as_strings
    };
}


// TODO: Might not handle syclic graphs
void NondeterministicFiniteAutomaton::initialize_eps_reachable()
{
    // Find all eps-edges
    std::map<State,std::vector<State>> edges;
    for (const auto& [k, target_states] : transition_function_) {
	auto source_state = k.first;
	auto symbol = k.second;

	if (symbol != empty_string_)
	    continue;
	
	edges.insert({source_state, target_states});
    }

/*     for (const auto& state : getStates()) { */
/* 	eps_reachable.insert({state, {state}}); */
/*     } */

    for (const auto& src_state : getStates()) {
	eps_reachable.insert({src_state, {}});
	std::queue<State> queue;

	queue.push(src_state);
	while (!queue.empty()) {
	    auto state = queue.front();
	    queue.pop();

	    eps_reachable[src_state].insert(state);
	    for (const auto& target_state : edges[state]) {
		queue.push(target_state);
	    }
	}

    }


}
