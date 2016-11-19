#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

typedef std::vector<std::vector<short> > LCPTable;

LCPTable CalculateLongestCommonPrefixes(const std::string &source_string)
{
    int string_length = source_string.length();
    LCPTable lcp = LCPTable(string_length + 1, std::vector<short>(string_length + 1, 0));
    for (int i = string_length - 1; i >= 0; --i)
    {
        for (int j = string_length - 1; j >= 0; --j)
        {
            if (source_string[i] == source_string[j])
            {
                lcp[i][j] = lcp[i + 1][j + 1] + 1;
            }
        }
    }
    return lcp;
}

int CalculateRepetitionsNumber(const std::string &source_string)
{
    LCPTable lcp = CalculateLongestCommonPrefixes(source_string);
    int max_repetitions_number = 1;
    int string_length = source_string.length();

    for (int repetition_str_length = 1; 
        repetition_str_length <= string_length / 2;
        ++repetition_str_length)
    {
        for (int i = 0; i < string_length - repetition_str_length * 2; ++i)
        {
            int prefix_length = lcp[i][i + repetition_str_length];
            int current_repetitions_number = (prefix_length / repetition_str_length) + 1;
            max_repetitions_number = std::max(max_repetitions_number, current_repetitions_number);
        }
    }
    return max_repetitions_number;
}


int main()
{
    std::string source_string;
    std::cin >> source_string;
    std::cout << CalculateRepetitionsNumber(source_string) << std::endl;
    return 0;
}
