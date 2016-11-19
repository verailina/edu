#include "LeftPushdownAutomaton.h"
#include "RightPushdownAutomaton.h"
#include <iostream>

const std::string kLeftKey = "-left";
const std::string kRightKey = "-right";
const std::string kSuccessString = "SUCCESS";
const std::string kFailureString = "FAILURE";
const std::string kLegend = "INPUT TAPE, STACK";
const std::string kHelpString = "Usage: -left|-right string";

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Incorrect arguments" << std::endl;
        std::cout << kHelpString << std::endl;
        return 1;
    }

    PushdownAutomaton *automaton = NULL;

    if (argv[1] == kLeftKey)
    {
        automaton = new LeftPushdownAutomaton();
    }

    if (argv[1] == kRightKey)
    {
        automaton = new RightPushdownAutomaton();
    }

    if (automaton != NULL)
    {
        if (automaton->ProcessString(argv[2]))
        {
            std::cout << kSuccessString << std::endl;
            std::cout << kLegend << std::endl;
            std::cout << automaton->GetHistory() << std::endl;
        }
        else
        {
            std::cout << kFailureString << std::endl;
        }
        delete automaton;
    }
    else
    {
        std::cout << "Incorrect key: " + std::string(argv[1]) << std::endl;
        std::cout << kHelpString << std::endl;
        return 1;
    }
    return 0;
}