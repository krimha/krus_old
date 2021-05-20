#include <krus/dfa.h>

// Assumes that we don't need to verify the accept states
// TODO Check that all mentioned symbols are in alphabet, and all mentioned states are in states
Status DeterministicFiniteAutomaton::verify()
{
    // Check that the transition function handles all combinations of symbols and states
    for (const auto& state : getStates()) {
	for (const auto& symbol : getAlphabet()) {
	    if (transition_function_.find({ state , symbol}) == transition_function_.end())
		return Status::TRANSITION_FUNCTION;
	}
    }

    // Start state is not valid
    if (std::find(states_.begin(), states_.end(), start_state_) == states_.end())
	return Status::START_STATE;

    return Status::OK;
}
