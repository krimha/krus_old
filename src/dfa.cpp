#include <krus/dfa.h>
// TODO: Check that the input character is valid
void DeterministicFiniteAutomaton::transition(Character character) 
{
    setState(transition_function_[std::pair(getCurrentState(), character)]);
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

    return true;
}
