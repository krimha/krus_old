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
    return iter_str(set);
}
