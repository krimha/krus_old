#include <catch2/catch.hpp>
#include <parselib/finite_state_machine.h>


TEST_CASE( "Create finite state machine" )
{
    FiniteStateMachine<std::string, char> machine{ 
	{"q1", "q2", "q3"},
	{ '0', '1' },

         {{{"q1", '0'}, "q1"}
         ,{{"q1", '1'}, "q2"}
         ,{{"q2", '0'}, "q3"}
         ,{{"q2", '1'}, "q2"}
         ,{{"q3", '0'}, "q2"}
         ,{{"q3", '1'}, "q2"}},
	 "q1",
	 { "q2" }
    };
}
