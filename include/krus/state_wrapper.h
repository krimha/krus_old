#pragma once
#include <string>
#include <unordered_set>

class StateWrapper {
public:
    using State = std::string;

    void insert(std::string str);
    std::unordered_set<State> getSet() const { return set; }
    std::string str();
    
private:
    std::unordered_set<State> set;
};
