#include <krus/dfa.h>
#include <krus/utils.h>

// Assumes that we don't need to verify the accept states
// TODO Check that all mentioned symbols are in alphabet, and all mentioned states are in states
Status DeterministicFiniteAutomaton::verify()
{
    // Check that the transition function handles all combinations of symbols and states
    for (const auto& state : getStates()) {
	for (const auto& symbol : getAlphabet()) {

	    std::pair<State,Character> p{ state, symbol };
	    if (!contains(transition_function_, p))
		return Status::TRANSITION_FUNCTION;
	}
    }

    // Start state is not valid
    if (!contains(states_, start_state_))
	return Status::START_STATE;

    return Status::OK;
}
