#pragma once
#include <map>
#include <iostream>
#include <vector>
#include <sstream>

#include <krus/dfa.h>

// TODO Some simple verification might be desirable for this. Check that all mentioned symbols are in alphabet, and all mentioned states are in states
// TODO Find a sensible way to handle empty string/chars
class NondeterministicFiniteAutomaton {
public:
    using State = std::string;
    using Character = char;
    using StateSet = std::vector<State>;
    using Alphabet = std::vector<Character>;

    // TODO: Would like to use unordered_map here, but will need to define a hash function for std::pair
    // TODO: Should this be an actual function that looks up values (or returns empty set)
    using TransitionFunction = std::map<std::pair<State,Character>, std::vector<State>>;
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
	, empty_string_{'\0'}
	{

	};

    StateSet getStates() { return states_; }
    Alphabet getAlphabet() { return alphabet_; };
    TransitionFunction getTransFunc() { return transition_function_; };
    State getStartState() { return start_state_; };
    StateSet getAcceptStates() { return accept_states_; };

    DeterministicFiniteAutomaton asDFA();

private:
    StateSet states_;
    Alphabet alphabet_;
    TransitionFunction transition_function_;
    State start_state_;
    StateSet accept_states_;
    Character empty_string_;
};

