#pragma once
#include "ArithmeticGrammar.h"
#include "PushdownAutomaton.h"

class LeftPushdownAutomaton : public PushdownAutomaton
{
public:
    LeftPushdownAutomaton();
    bool ProcessString(const std::string &input_string);   
    const std::string &GetHistory() const;

private:
    std::string history_;
    static const ArithmeticGrammar kGrammar;

    bool Process(std::string current_string, Stack current_stack, std::string &history);
    std::string GetCurrentStateString(const std::string &current_string, const Stack current_stack);
};

