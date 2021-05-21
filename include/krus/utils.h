#pragma once
#include <string>
#include <sstream>
#include <algorithm>

#include <krus/dfa.h>

template<typename T>
std::string iter_str(const T& iter)
{
    std::stringstream ss;
    ss << "{ ";
    for (const auto& x : iter) {
	ss << x << ' ';
    }
    ss << '}';
    return ss.str();
}

template<typename Container, typename Element>
bool contains(const Container& container, const Element& elem)
{
    // TODO: Possible to unify these
    // TODO: Maybe make more general (i.e. work with maps)
    if constexpr (std::is_same_v<Container, DeterministicFiniteAutomaton::TransitionFunction>) {
	return container.find(elem) != container.cend();
    }
    else {
	return std::find(container.cbegin(), container.cend(), elem) != container.cend();
    }
}
