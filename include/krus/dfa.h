#pragma once
#include <vector>
#include <map>
#include <string>
#include <algorithm>


enum class Status {
    TRANSITION_FUNCTION,
    START_STATE,
    OK
};


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
    {};

    StateSet getStates() { return states_; }
    Alphabet getAlphabet() { return alphabet_; };
    TransitionFunction getTransFunc() { return transition_function_; };
    State getStartState() { return start_state_; };
    StateSet getAcceptStates() { return accept_states_; };

    template<typename Iterable>
    bool match(Iterable input_string);

    Status verify();


private:
    StateSet states_;
    Alphabet alphabet_;
    TransitionFunction transition_function_;
    State start_state_;
    StateSet accept_states_;
};

template<typename Iterable>
bool DeterministicFiniteAutomaton::match(Iterable input_string)
{
    auto current_state = start_state_;
    for (const auto& character : input_string) {
	current_state = transition_function_[std::pair(current_state, character)];
    }

    auto it = std::find(accept_states_.begin(), accept_states_.end(), current_state);
    auto result = it != accept_states_.end();

    return result;
}


