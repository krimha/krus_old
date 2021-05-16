#pragma once
#include <vector>
#include <map>

// TODO: Templating. Should currently only acces cases where string string. However, would like e.g. std::set<std::string>. This is difficult, however:
// - Need a std::set<std::set<std::string>> type, which doesn't seem to be possible without defining a hash function for sets.
// - lookups in transitionfunction does not seem to work
class DeterministicFiniteAutomaton {
public:
    using State = std::string;
    using Character = char;
    using StateSet = std::vector<State>;
    using Alphabet = std::vector<Character>;

    // TODO: Would like to use unordered_map here, but will need to define a hash function for std::pair
    using TransitionFunction = std::map<std::pair<State,Character>,State>;
    DeterministicFiniteAutomaton(
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
	, current_state_{start_state}
    {};

    StateSet getStates() { return states_; }
    Alphabet getAlphabet() { return alphabet_; };
    TransitionFunction getTransFunc() { return transition_function_; };
    State getStartState() { return start_state_; };
    StateSet getAcceptStates() { return accept_states_; };

    State getCurrentState() { return current_state_; };

    void setState(State state) { current_state_ = state; };
    void reset() { setState(start_state_); };

    void transition(Character character);

    template<typename Iterable>
    bool match(Iterable input_string);

    bool verify();


private:
    StateSet states_;
    Alphabet alphabet_;
    TransitionFunction transition_function_;
    State start_state_;
    StateSet accept_states_;

    State current_state_;
};

// TODO: Check that the input character is valid
void DeterministicFiniteAutomaton::transition(Character character) 
{
    setState(transition_function_[std::pair(getCurrentState(), character)]);
}

template<typename Iterable>
bool DeterministicFiniteAutomaton::match(Iterable input_string)
{
    for (const auto& character : input_string) {
	transition(character);
    }

    auto it = std::find(accept_states_.begin(), accept_states_.end(), getCurrentState());
    auto result = it != accept_states_.end();
    /* auto result = accept_states_.find(getCurrentState()) != accept_states_.end(); */

    reset();
    return result;
}


// Assumes that we don't need to verify the accept states
// TODO Check that all mentioned symbols are in alphabet, and all mentioned states are in states
bool DeterministicFiniteAutomaton::verify()
{
    // Check that the transition function handles all combinations of symbols and states
    for (const auto& state : getStates()) {
	for (const auto& symbol : getAlphabet()) {
	    if (transition_function_.find({ state , symbol}) == transition_function_.end())
		return false;
	}
    }

    // Start state is not valid
    if (std::find(states_.begin(), states_.end(), start_state_) == states_.end())
	return false;

    // if (states_.find(start_state_) == states_.end()) {
    //     return false;
    // }


    return true;
}
