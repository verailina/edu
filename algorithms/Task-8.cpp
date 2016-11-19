#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <assert.h>

typedef std::vector<std::string> Rule;
typedef std::vector<Rule> Grammar;
typedef std::pair<int, int> Position;

int kMaxVariablesNumber = 26;

enum SymbolType
{
    EMPTY,
    TERMINAL,
    VARIABLE
};

Grammar ReadGrammar()
{
    Grammar grammar = Grammar(kMaxVariablesNumber, Rule());
    int rules_number = 0;
    std::cin >> rules_number;

    for (int i = 0; i < rules_number; ++i)
    {
        char variable = 0;
        std::cin >> variable;
        std::string right_part;
        char arrow = 0;
        std::cin >> arrow;
        assert(arrow == '-');
        std::cin >> arrow;
        assert(arrow == '>');
        std::cin >> right_part;
        grammar[variable - 'A'].push_back(right_part);
    }

    return grammar;
}



SymbolType GetSymbolType(char symbol)
{
    if (symbol == '$')
    {
        return SymbolType::EMPTY;
    }
    if (symbol >= 'a' && symbol <= 'z')
    {
        return SymbolType::TERMINAL;
    }
    return SymbolType::VARIABLE;
}

Grammar SelectGeneratingVariables(const Grammar &grammar)
{
    std::vector<std::vector<Position>> rules_generates_variable =
        std::vector<std::vector<Position>>(kMaxVariablesNumber, std::vector<Position>());
    std::vector<std::vector<int>> neterminals_number =
        std::vector<std::vector<int>>(grammar.size(), std::vector<int>());

    for (int i = 0; i < grammar.size(); ++i)
    {
        for (int j = 0; j < grammar[i].size(); ++j)
        {
            for (int k = 0; k < grammar[i][j].size(); ++k)
            {
                if (GetSymbolType(grammar[i][j][k]) == SymbolType::VARIABLE)
                {
                    rules_generates_variable[grammar[i][j][k] - 'A'].push_back(Position(i, j));
                    neterminals_number[]
                }
            }
        }
    }

}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    Grammar grammar = ReadGrammar();

    int min_string_size = 0;
    std::cin >> min_string_size;

    return 0;
}