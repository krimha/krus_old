#include <vector>
#include <algorithm>

#include <krus/state_wrapper.h>
#include <krus/utils.h>

void StateWrapper::insert(std::string str)
{
    set.insert(str);
}


std::string StateWrapper::str()
{
    std::vector<std::string> v;
    for (const auto& x : set) {
	v.push_back(x);
    }
    std::sort(v.begin(), v.end());
    return iter_str(v);
}
