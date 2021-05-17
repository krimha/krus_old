#include <vector>
#include <algorithm>

#include <krus/state_wrapper.h>
#include <krus/utils.h>

void StateWrapper::insert(std::string str)
{
    set.insert(str);
}


std::string StateWrapper::str() const
{
    return iter_str(set);
}

bool StateWrapper::contains(const State &state) const
{
    if (set.find(state) == set.end())
	return false;
    return true;
}
