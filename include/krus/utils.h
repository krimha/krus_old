#pragma once
#include <string>
#include <sstream>

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
