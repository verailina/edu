#include "ArithmeticGrammar.h"

const std::string ArithmeticGrammar::kNonterminals = "ETF";
const std::string ArithmeticGrammar::kTerminals = "a+*()";

ArithmeticGrammar::ArithmeticGrammar()
{
    for (int i = 0; i < kNonterminals.size(); ++i)
    {
        rules[kNonterminals[i]] = std::vector<std::string>();
    }
 
    rules['E'].push_back("E+T");
    rules['E'].push_back("T");
    rules['T'].push_back("T*F");
    rules['T'].push_back("F");
    rules['F'].push_back("(E)");
    rules['F'].push_back("a");

    inverted_rules["E+T"] = 'E';
    inverted_rules["T"] = 'E';
    inverted_rules["T*F"] = 'T';
    inverted_rules["F"] = 'T';
    inverted_rules["(E)"] = 'F';
    inverted_rules["a"] = 'F';
}

/*Returns the right part a of the rules A->a*/
std::vector<std::string> ArithmeticGrammar::GetRules(char symbol) const
{
    return rules.at(symbol);
}

//Returns the left part A of the rules A->a
char ArithmeticGrammar::GetLeftPart(const std::string &chain) const
{
    return inverted_rules.at(chain);
}

//Tests if the rigt part chain presents in the grammar
bool ArithmeticGrammar::ContainsChain(const std::string &chain) const
{
    return inverted_rules.count(chain) != 0;
}

bool ArithmeticGrammar::IsTerminal(char symbol)
{
    if (kTerminals.find(symbol) != std::string::npos)
    {
        return true;
    }
    return false;
}

bool ArithmeticGrammar::IsNonterminal(char symbol)
{
    if (kNonterminals.find(symbol) != std::string::npos)
    {
        return true;
    }
    return false;
}



