#pragma once
#include <unordered_set>
#include <map>

// TODO Some simple verification might be desirable for this. Check that all mentioned symbols are in alphabet, and all mentioned states are in states
template<typename State_, typename Character_>
class NondeterministicFiniteAutomaton {
public:
    using State = State_;
    using Character = Character_;
    using StateSet = std::unordered_set<State>;
    using Alphabet = std::unordered_set<Character>;

    // TODO: Would like to use unordered_map here, but will need to define a hash function for std::pair
    using TransitionFunction = std::map<std::pair<State,Character>, std::unordered_set<State>>;
    NondeterministicFiniteAutomaton(
	    StateSet states,
	    Alphabet alphabet,
	    TransitionFunction f,
	    State start_state,
	    StateSet accept_states
	    ) 
	: states_{states} 
	, alphabet_{alphabet} 
	, transition_function_{f} 
	, start_state_{start_state} 
	, accept_states_{accept_states} 
    {};

    StateSet getStates() { return states_; }
    Alphabet getAlphabet() { return alphabet_; };
    TransitionFunction getTransFunc() { return transition_function_; };
    State getStartState() { return start_state_; };
    StateSet getAcceptStates() { return accept_states_; };

private:
    StateSet states_;
    Alphabet alphabet_;
    TransitionFunction transition_function_;
    State start_state_;
    StateSet accept_states_;
};
