#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

typedef std::vector<int> SuffixArray;

class SuffixArrayBuilder
{
private:
    std::vector<int> labels_;
    int substring_length_;
    int string_length_;
    int keys_number_;

    std::vector<int> GetCharactersLabels(const std::string &source_string)
    {
        std::vector<int> labels = std::vector<int>(source_string.size() * 2, 0);
        for (int i = 0; i < source_string.length(); ++i)
        {
            labels[i] = source_string[i] - 'a' + 1;
            keys_number_ = std::max(labels[i] + 1, keys_number_);
        }
        return labels;
    }

    std::vector<int> CountingSort(const std::vector<int> &indices_order, 
                                  const std::vector<int> &keys)
    {
        std::vector<int> keys_counters = std::vector<int>(keys_number_, 0);
        std::vector<int> keys_indices = std::vector<int>(keys_number_, 0);
        
        for (int i = 0; i < string_length_; ++i)
        {
            assert(keys[i] < keys_number_ && keys[i] >= 0);
            keys_counters[keys[i]]++;
        }

        int current_key_index = 0;
        for (int i = 0; i < keys_number_; ++i)
        {
            keys_indices[i] = current_key_index;
            current_key_index += keys_counters[i];
        }

        std::vector<int> new_indices_order = std::vector<int>(current_key_index + 1, 0);

        for (int i = 0; i < string_length_; ++i)
        {
            int current_key = keys[indices_order[i]];
            int current_key_index = keys_indices[current_key]++;
            new_indices_order[current_key_index] = indices_order[i];
        }

        return new_indices_order;
    }

    void RecountLabels()
    {
        int lables_number = labels_.size();

        std::vector<int> second_labels = std::vector<int>(lables_number - substring_length_);
        for (int i = 0; i < lables_number - substring_length_; ++i)
        {
            second_labels[i] = labels_[i + substring_length_];
        }

        std::vector<int> trivial_order;
        for (int i = 0; i < second_labels.size(); ++i)
        {
            trivial_order.push_back(i);
        }

        std::vector<int> second_label_oreder = CountingSort(trivial_order, second_labels);
        std::vector<int> first_label_order = CountingSort(second_label_oreder, labels_);

        std::vector<int> new_labels = std::vector<int>(lables_number, 0);

        int current_label = 1;
        for (int i = 0; i < string_length_; ++i)
        {
            new_labels[first_label_order[i]] = current_label;
            if (labels_[first_label_order[i]] !=
                labels_[first_label_order[i + 1]] ||
                labels_[first_label_order[i] + substring_length_] !=
                labels_[first_label_order[i + 1] + substring_length_])
            {
                current_label++;
            }
        }
        keys_number_ = current_label + 1;
        labels_ = new_labels;
    }

    SuffixArray BuidSuffixArrayFromLabels()
    {
        SuffixArray suffix_array = SuffixArray(labels_.size(), 0);

        for (int i = 0; i < labels_.size(), labels_[i] != 0; ++i)
        {
            suffix_array[labels_[i] - 1] = i;
        }

        return suffix_array;
    }

public:
    SuffixArray BuildSuffixArray(const std::string &source_string)
    {     
        keys_number_ = 0;
        labels_ = GetCharactersLabels(source_string);
        string_length_ = source_string.length();

        for (substring_length_ = 1; 
            substring_length_ < string_length_; 
            substring_length_ *= 2)
        {
            RecountLabels();
        }

        return BuidSuffixArrayFromLabels();
    }

};

std::string BurrowsWheelerTransform(const std::string &source_string)
{
    std::string double_string = source_string + source_string;
    SuffixArray suffix_array = SuffixArrayBuilder().BuildSuffixArray(double_string);
    std::string result = "";
    for (int i = 0; i < double_string.size(); ++i)
    {
        int suffix_size = suffix_array[i] + source_string.size();
        if (suffix_size < double_string.size())
        {
            result += double_string[suffix_size - 1];
        }
    }
    return result;
}

int main()
{
    std::string source_string;
    std::cin >> source_string;
    std::cout << BurrowsWheelerTransform(source_string);

    return 0;
}
