#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <assert.h>
#include <algorithm>

class CommentsDeleter
{
    enum State
    {
        CODE,
        COMMENT,
        CHAR_STRING
    };

private:
    State state_;
    
    static const char kSlash = '/';
    static const char kBackSlash = '\\';
    static const char kStar = '*';
    static const char kQuotes = '"';

public:
    CommentsDeleter()
    {
        state_ = CODE;
    }

    void DeleteComments(std::istream &input_stream, std::ostream &output_stream)
    {
        std::string text_line;
        while (std::getline(input_stream, text_line))
        {
            std::string concatinated_line = text_line;
            while (!text_line.empty() &&
                    text_line[text_line.size() - 1] == kBackSlash &&
                    !input_stream.eof())
            {
                concatinated_line.pop_back();
                std::getline(input_stream, text_line);
                concatinated_line += text_line;
            }

            std::string current_line = DeleteComments(concatinated_line);
            if (!current_line.empty())
            {
                output_stream << current_line << std::endl;
            }
        }
    }

    std::string DeleteComments(const std::string &text_line)
    {
        bool is_empty_line = true;
        std::string text_line_without_comments;
        const int text_line_size = text_line.size();
        for (int i = 0; i < text_line_size; ++i)
        {
            switch (state_)
            {
            case COMMENT:
            {
                if (text_line[i] == kStar &&
                    i + 1 < text_line_size &&
                    text_line[i + 1] == kSlash)
                {
                    state_ = CODE;
                    i++;
                }
                break;
            }
            case CODE:
            {
                if (text_line[i] == kQuotes)
                {
                    state_ = CHAR_STRING;
                }

                if (text_line[i] == kSlash &&
                    i + 1 < text_line_size &&
                    text_line[i + 1] == kSlash)
                {
                    return is_empty_line ? "" : text_line_without_comments;
                }

                if (text_line[i] == kSlash &&
                    i + 1 < text_line_size &&
                    text_line[i + 1] == kStar)
                {
                    state_ = COMMENT;
                    i++;
                    break;
                }
                is_empty_line = is_empty_line && (std::isprint(text_line[i]) ? false : true);
                text_line_without_comments += text_line[i];
                break;
            }
            case CHAR_STRING:
            {
                if (text_line[i] == kBackSlash &&
                    i + 1 < text_line_size)
                {
                    i++;
                    is_empty_line = false;
                    text_line_without_comments += kBackSlash;
                    text_line_without_comments += text_line[i];
                    break;
                }
                if(text_line[i] == kQuotes)
                {
                    state_ = CODE;
                }
                is_empty_line = is_empty_line && (std::isprint(text_line[i]) ? false : true);
                text_line_without_comments += text_line[i];
            }
            }
        }
        return is_empty_line ? "" : text_line_without_comments;
    }
};

bool CompareFiles(const std::string &first_file_name, const std::string &second_file_name)
{
    std::ifstream first_file(first_file_name);
    std::ifstream second_file(second_file_name);
    std::string first_file_line;
    std::string second_file_line;
    bool result = true;

    while (!first_file.eof() && !second_file.eof())
    {
        std::getline(first_file, first_file_line);
        std::getline(second_file, second_file_line);
        if (first_file_line != second_file_line)
        {
            result = false;
            break;
        }
    }

    if (!first_file.eof() || !second_file.eof())
    {
        result = false;
    }

    first_file.close();
    second_file.close();
    return result;
}

void DeleteCommentsTest(const std::string &input_file_name, 
                        const std::string &result_file_name,
                        const std::string &expected_file_name)
{
    std::ifstream input_file(input_file_name);
    std::ifstream expected_file(expected_file_name);
    std::ofstream result_file_output(result_file_name);

    CommentsDeleter().DeleteComments(input_file, result_file_output);
    result_file_output.close();
    assert(CompareFiles(result_file_name, expected_file_name));
    input_file.close();
    expected_file.close();
}

int main()
{
    DeleteCommentsTest("input.txt", "result.txt", "expected.txt");
    return 0;
}