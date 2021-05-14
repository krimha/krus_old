#pragma once
#include <unordered_set>
#include <map>

template<typename State, typename Character>
class FiniteStateMachine {
public:
    using StateSet = std::unordered_set<State>;
    using Alphabet = std::unordered_set<Character>;
    // TODO: Would like to use unordered_map here, but will need to define a hash function for std::pair
    using TransitionFunction = std::map<std::pair<State,Character>,State>;
    FiniteStateMachine(
	    StateSet states,
	    Alphabet alphabet,
	    TransitionFunction f,
	    State start_state,
	    StateSet accept_states) {};

};
