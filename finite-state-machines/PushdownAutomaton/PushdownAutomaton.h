#pragma once
#include "ArithmeticGrammar.h"
#include <string>
#include <stack>
typedef std::stack<char> Stack;


class PushdownAutomaton
{
public:
    virtual bool ProcessString(const std::string &input_string)=0;
    virtual const std::string &GetHistory() const=0;
    static const char kStatesDelimiter = '\n';

private:
    virtual bool Process(std::string current_string, Stack current_stack, std::string &history)=0;
    virtual std::string GetCurrentStateString(const std::string &current_string, const Stack current_stack)=0;
};

