#include <catch2/catch.hpp>
#include <krus/dfa.h>
#include <krus/nfa.h>
#include <krus/state_wrapper.h>
#include <krus/utils.h>



TEST_CASE( "Deterministic finite state machine tests" )
{
    using DFA = DeterministicFiniteAutomaton;
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
    }

    SECTION ("Test match")
    {
	REQUIRE(machine.match(std::string{"1101"}) == true );
	// Test that no state is stored between calls
	REQUIRE(machine.match(std::string{"110"})  == false);
	REQUIRE(machine.match(std::string{"1"})  == true);
    }
    
    SECTION("Test valid")
    {
	REQUIRE(machine.verify() == Status::OK);
    }


    SECTION ("Missing arrow")
    {
	// {{{"q1", '0'}, "q1"}
	func.erase(func.begin());
	DFA machine{ states, alpha, func, start_state, accept_states };

	REQUIRE( machine.verify() == Status::TRANSITION_FUNCTION );
    }

    // This test is  pretty much the same
    SECTION ("Extra alphabet char")
    {
	alpha.push_back('x');
	DFA machine{ states, alpha, func, start_state, accept_states };

	REQUIRE( machine.verify() == Status::TRANSITION_FUNCTION );
    }

    SECTION ("Start state not in state")
    {
	DFA machine{ states, alpha, func, "q_error", accept_states };

	REQUIRE( machine.verify() == Status::START_STATE );
    }
}


TEST_CASE( "Nondeterministic finite state machine tests" )
{
    using NFA = NondeterministicFiniteAutomaton;
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


TEST_CASE ( "Test nondeterministic finite state machine" )
{
    using NFA = NondeterministicFiniteAutomaton;
    NFA::StateSet states = {"q1", "q2", "q3"};
    NFA::Alphabet alpha  = { 'a', 'b' };
    NFA::TransitionFunction func =  
    { {{"q1",  'b'}, {"q2"}},
      {{"q1", '\0'}, {"q3"}},
      {{"q2",  'a'}, {"q2", "q3"}},
      {{"q2",  'b'}, {"q3"}},
      {{"q3",  'a'}, {"q1"}} };
    NFA::State start_state = "q1";
    NFA::StateSet accept_states = {"q1"};

    NFA nfa { states, alpha, func, start_state, accept_states };
    REQUIRE(contains(nfa.getEpsReachable()["q1"], "q1"));
    REQUIRE(contains(nfa.getEpsReachable()["q2"], "q2"));
    REQUIRE(contains(nfa.getEpsReachable()["q3"], "q3"));

    REQUIRE(contains(nfa.getEpsReachable()["q1"], "q3"));


    auto dfa = nfa.asDFA();
    REQUIRE(dfa.verify() == Status::OK);
    REQUIRE(dfa.getStartState() == "{ q1 q3 }");
    REQUIRE(dfa.getAlphabet() == alpha);


    // If the NFA has k states, the equivalent DFA has 2^k states
    size_t k = nfa.getStates().size();
    REQUIRE(dfa.getStates().size() == (1 << k));

    // TODO: Should also test that the accept states are valid, but that is difficult with the current implementation
    REQUIRE(dfa.getAcceptStates().size() == 4);

    /* for (const auto& [k,out] : dfa.getTransFunc()) { */
	/* const auto&[in,sym] = k; */
	
	/* std::cout << in << ' ' << sym << ' ' << out << '\n'; */
    /* } */

    REQUIRE(dfa.match(std::string("")));
    REQUIRE(dfa.match(std::string("a")));
    REQUIRE(dfa.match(std::string("baba")));
    REQUIRE(dfa.match(std::string("baa")));

    REQUIRE_FALSE(dfa.match(std::string("b")));
    REQUIRE_FALSE(dfa.match(std::string("bb")));
    REQUIRE_FALSE(dfa.match(std::string("babba")));
}

TEST_CASE ("Test StateWrapper")
{
    SECTION ("Straight forward")
    {
	StateWrapper s;
	s.insert("q1");
	s.insert("q1");
	s.insert("q2");
	s.insert("q1");
	s.insert("q3");

	REQUIRE(s.str() == "{ q1 q2 q3 }");
    }

    SECTION ("Empty set")
    {
	StateWrapper s;
	REQUIRE(s.str() == "{ }");
    }
}

TEST_CASE ("Test contains")
{
    std::vector<int> v { 3, 6, 4, 1, 5 };
    std::set<int> s { 3, 6, 4, 1, 5 };
    
    REQUIRE(contains(v, 4));
    REQUIRE(contains(v, 5));
    REQUIRE_FALSE(contains(v, 9));
    REQUIRE_FALSE(contains(v, 10));

    REQUIRE(contains(s, 4));
    REQUIRE(contains(s, 5));
    REQUIRE_FALSE(contains(s, 9));
    REQUIRE_FALSE(contains(s, 10));
}
