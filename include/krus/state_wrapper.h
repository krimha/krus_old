#pragma once
#include <string>
#include <set>

class StateWrapper {
public:
    using State = std::string;

    void insert(std::string str);
    std::set<State> getSet() const { return set; }
    std::string str();
    
private:
    std::set<State> set;
};
