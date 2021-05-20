#pragma once
#include <string>
#include <sstream>
#include <algorithm>

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
    return std::find(container.cbegin(), container.cend(), elem) != container.cend();
}
