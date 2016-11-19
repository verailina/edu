#pragma once
#include <string>
#include <vector>
#include <map>

typedef std::map<char, std::vector<std::string>> Rules;
typedef std::map<std::string, char> InvertedRules;

class ArithmeticGrammar
{
public:
    ArithmeticGrammar();

    static bool IsTerminal(char symbol);
    static bool IsNonterminal(char symbol);
    std::vector<std::string> GetRules(char symbol) const;
    char GetLeftPart(const std::string &chain) const;
    bool ContainsChain(const std::string &chain) const;

private:
    Rules rules;
    InvertedRules inverted_rules;
    static const std::string kNonterminals;
    static const std::string kTerminals;
};

