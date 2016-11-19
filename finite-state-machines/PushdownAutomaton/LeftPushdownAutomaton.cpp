#include "LeftPushdownAutomaton.h"

const ArithmeticGrammar LeftPushdownAutomaton::kGrammar = ArithmeticGrammar();

LeftPushdownAutomaton::LeftPushdownAutomaton()
{
}

/*Initiates the recursive procedure of string processing*/
bool LeftPushdownAutomaton::ProcessString(const std::string &input_string)
{
    Stack start_stack;
    start_stack.push('E');
    return Process(input_string, start_stack, GetCurrentStateString(input_string, start_stack));
}

/*Recursive procedure of string processing*/
bool LeftPushdownAutomaton::Process(std::string current_string, Stack current_stack, std::string &history)
{
    //There is not empty terminal in Grammar and every nonterminal symbol 
    //corresponds at least one terminal symbol
    if (current_stack.size() > current_string.length())
    {
        return false;
    }

    //Delete identical terminals from the top of current stack_and of the 
    //beginning of current_string
    while (current_string.size() > 0 && !current_stack.empty() &&
           ArithmeticGrammar::IsTerminal(current_stack.top()))
    {
        if (current_string[0] == current_stack.top())
        {
            current_string = current_string.substr(1, std::string::npos);
            current_stack.pop();
            history += kStatesDelimiter + GetCurrentStateString(current_string, current_stack);
        }
        else
        {
            return false;
        }
    }

    //Replace the nonterminal symbol at the top of stack on the right part 
    //of all possible rules
    if (!current_stack.empty() && ArithmeticGrammar::IsNonterminal(current_stack.top()))
    {
        std::vector<std::string> rules = kGrammar.GetRules(current_stack.top());
        current_stack.pop();
        for (int i = 0; i < rules.size(); ++i)
        {
            Stack new_stack = current_stack;
            for (int j = rules[i].size() - 1; j >= 0; --j)
            {
                new_stack.push(rules[i][j]);
            }
            if (Process(current_string, new_stack, history + kStatesDelimiter +GetCurrentStateString(current_string, new_stack)))
            {
                return true;
            }
        }
    }

    //Success termination
    if (current_string.empty() && current_stack.empty())
    {
        history_ = history;
        return true;
    }
    return false;
}


/*Converts the current state of pushdown automaton to the string*/
std::string LeftPushdownAutomaton::GetCurrentStateString(const std::string &current_string, Stack current_stack)
{
    std::string state = current_string;
    state += ", ";
    while (!current_stack.empty())
    {
        state += current_stack.top();
        current_stack.pop();
    }
    return state;
}

/*Returns the history of the string processing.
In the case of failure termination returns empty string*/
const std::string & LeftPushdownAutomaton::GetHistory() const
{
    return history_;
}

