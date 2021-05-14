#pragma once
#include <unordered_set>
#include <map>

template<typename State_, typename Character_>
class FiniteStateMachine {
public:
    using State = State_;
    using Character = Character_;
    using StateSet = std::unordered_set<State>;
    using Alphabet = std::unordered_set<Character>;

    // TODO: Would like to use unordered_map here, but will need to define a hash function for std::pair
    using TransitionFunction = std::map<std::pair<State,Character>,State>;
    FiniteStateMachine(
	    StateSet states,
	    Alphabet alphabet,
	    TransitionFunction f,
	    State start_state,
	    StateSet accept_states
	    ) 
	: states_{states} 
	, alphabet_{alphabet} 
	, f_{f} 
	, start_state_{start_state} 
	, accept_states_{accept_states} 
	, current_state_{start_state}
    {};

    StateSet getStates() { return states_; }
    Alphabet getAlphabet() { return alphabet_; };
    TransitionFunction getTransFunc() { return f_; };
    State getStartState() { return start_state_; };
    StateSet getAcceptStates() { return accept_states_; };

    State getCurrentState() { return current_state_; };

    void transition(const Character& character);

private:
    StateSet states_;
    Alphabet alphabet_;
    TransitionFunction f_;
    State start_state_;
    StateSet accept_states_;

    State current_state_;
};



