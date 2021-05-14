#include <catch2/catch.hpp>
#include <parselib/finite_state_machine.h>


TEST_CASE( "Create finite state machine" )
{
    using DFA = FiniteStateMachine<std::string, char>;
    DFA::StateSet states = {"q1", "q2", "q3"};
    DFA::Alphabet alpha  = { '0', '1' };
    DFA::TransitionFunction func =  
	{{{"q1", '0'}, "q1"}
	,{{"q1", '1'}, "q2"}
	,{{"q2", '0'}, "q3"}
	,{{"q2", '1'}, "q2"}
	,{{"q3", '0'}, "q2"}
	,{{"q3", '1'}, "q2"}};
    DFA::State start_state = "q1";
    DFA::StateSet accept_states = {"q2"};

    DFA machine{ states, alpha, func, start_state, accept_states };

    REQUIRE(machine.getStates() == states);
    REQUIRE(machine.getAlphabet() ==  alpha);
    REQUIRE(machine.getTransFunc() ==  func);
    REQUIRE(machine.getStartState() ==  start_state);
    REQUIRE(machine.getAcceptStates() == accept_states);
    REQUIRE(machine.getCurrentState() == start_state);
}
