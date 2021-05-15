#include <catch2/catch.hpp>
#include <krus/dfa.h>
#include <krus/nfa.h>


TEST_CASE( "Deterministic finite state machine tests" )
{
    using DFA = DeterministicFiniteAutomaton<std::string, char>;
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

    SECTION ("Check that initialization works")
    {
	REQUIRE(machine.getStates() == states);
	REQUIRE(machine.getAlphabet() ==  alpha);
	REQUIRE(machine.getTransFunc() ==  func);
	REQUIRE(machine.getStartState() ==  start_state);
	REQUIRE(machine.getAcceptStates() == accept_states);
	REQUIRE(machine.getCurrentState() == start_state);
    }

    SECTION ("Test setState")
    {
	machine.setState("q3");
	REQUIRE(machine.getCurrentState() == "q3");
    }

    SECTION ("Test reset")
    {
	machine.setState("q3");
	machine.reset();
	REQUIRE(machine.getCurrentState() == start_state);
    }

    SECTION ("Test transition")
    {
	REQUIRE(machine.getCurrentState() == start_state);
	machine.transition('0');
	REQUIRE(machine.getCurrentState() == start_state);
	machine.transition('1');
	REQUIRE(machine.getCurrentState() == "q2");
	machine.transition('1');
	REQUIRE(machine.getCurrentState() == "q2");
	machine.transition('0');
	REQUIRE(machine.getCurrentState() == "q3");
	machine.transition('0');
	REQUIRE(machine.getCurrentState() == "q2");

    }

    SECTION ("Test match")
    {
	REQUIRE(machine.match(std::string{"1101"}) == true );
	// Test that it is reset
	REQUIRE(machine.getCurrentState() == start_state);
	REQUIRE(machine.match(std::string{"110"})  == false);
	REQUIRE(machine.getCurrentState() == start_state);
	REQUIRE(machine.match(std::string{"1"})  == true);
	REQUIRE(machine.getCurrentState() == start_state);
    }
    
    SECTION("Test valid")
    {
	REQUIRE(machine.verify());
    }


    SECTION ("Missing arrow")
    {
	// {{{"q1", '0'}, "q1"}
	func.erase(func.begin());
	DFA machine{ states, alpha, func, start_state, accept_states };

	REQUIRE( machine.verify() == false );
    }

    // This test is  pretty much the same
    SECTION ("Extra alphabet char")
    {
	alpha.insert('x');
	DFA machine{ states, alpha, func, start_state, accept_states };

	REQUIRE( machine.verify() == false );
    }

    SECTION ("Start state not in state")
    {
	DFA machine{ states, alpha, func, "q_error", accept_states };

	REQUIRE( machine.verify() == false );
    }
}


TEST_CASE( "Nondeterministic finite state machine tests" )
{
    using NFA = NondeterministicFiniteAutomaton<std::string, char>;
    NFA::StateSet states = {"q1", "q2", "q3"};
    NFA::Alphabet alpha  = { '0', '1' };
    NFA::TransitionFunction func =  
	{{{"q1", '0'}, {"q1"}}
	,{{"q1", '1'}, {"q2"}}
	,{{"q2", '0'}, {"q3"}}
	,{{"q2", '1'}, {"q2"}}
	,{{"q3", '0'}, {"q2"}}
	,{{"q3", '1'}, {"q2"}}};
    NFA::State start_state = "q1";
    NFA::StateSet accept_states = {"q2"};

    NFA machine{ states, alpha, func, start_state, accept_states };

    SECTION ("Check that initialization works")
    {
	REQUIRE(machine.getStates() == states);
	REQUIRE(machine.getAlphabet() ==  alpha);
	REQUIRE(machine.getTransFunc() ==  func);
	REQUIRE(machine.getStartState() ==  start_state);
	REQUIRE(machine.getAcceptStates() == accept_states);
    }
}
