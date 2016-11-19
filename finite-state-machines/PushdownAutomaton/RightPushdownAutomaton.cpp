#include "RightPushdownAutomaton.h"
#include <algorithm>

const ArithmeticGrammar RightPushdownAutomaton::kGrammar = ArithmeticGrammar();
const int kMaxChainLength = 3;

RightPushdownAutomaton::RightPushdownAutomaton()
{
}

/*Initiates the recursive procedure of string processing*/
bool RightPushdownAutomaton::ProcessString(const std::string &input_string)
{
    Stack start_stack;
    return Process(input_string, start_stack, GetCurrentStateString(input_string, start_stack));
}

/*Recursive procedure of string processing*/
bool RightPushdownAutomaton::Process(std::string current_string, Stack current_stack, std::string &history)
{
    //Success termination
    if (current_string.empty() && current_stack.size() == 1 &&
        current_stack.top() == 'E')
    {
        history_ = history;
        return true;
    }
    
    //Convolution : Replace 1-, 2-, 3-size chains at the top of stack on the 
    //left part of the corresponding rule
    if (!current_stack.empty())
    {
        int chain_length = std::min((int)current_stack.size(), kMaxChainLength);
        for (int length = 1; length <= chain_length; ++length)
        {
            Stack new_stack = current_stack;
            std::string chain;
            for (int i = 0; i < length; ++i)
            {
                chain = new_stack.top() + chain;
                new_stack.pop();
            }

            if (kGrammar.ContainsChain(chain))
            {
                new_stack.push(kGrammar.GetLeftPart(chain));
                if (Process(current_string, new_stack, 
                    history + kStatesDelimiter + GetCurrentStateString(current_string, new_stack)))
                {
                    return true;
                }
            }
        }
    }

    //Carry : Push one symbol from the string to the top of stack
    if (!current_string.empty())
    {
        Stack new_stack = current_stack;
        new_stack.push(current_string[0]);
        std::string new_string = current_string.substr(1, std::string::npos);
        if (Process(new_string, new_stack, 
            history + kStatesDelimiter + GetCurrentStateString(new_string, new_stack)))
        {
            return true;
        }
    }

    return false;
}

/*Converts the current state of pushdown automaton to the string*/
std::string RightPushdownAutomaton::GetCurrentStateString(const std::string &current_string, Stack current_stack)
{
    std::string state = "";
    while (!current_stack.empty())
    {
        state = current_stack.top() + state;
        current_stack.pop();
    }
    return current_string + ", " + state;
}

/*Returns the history of the string processing.
In the case of failure termination returns empty string*/
const std::string & RightPushdownAutomaton::GetHistory() const
{
    return history_;
}


