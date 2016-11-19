#include <vector>
#include <iostream>
#include <set>

typedef std::vector<std::vector<int>> Matrix;
typedef std::vector<std::vector<bool>> MatrixBool;

typedef std::vector<std::vector<std::set<int> > > ReversedFSM;

class FiniteStateMachine
{
private:
    Matrix transitions_;
    int states_number_;
    int alphabet_size_;
    std::set<int> terminal_states_;
    static const int kStartState = 0;

    void MarkReachableStates(int current_state, std::vector<bool> &reachable_flags) const
    {
        reachable_flags[current_state] = true;
        for (int i = 0; i < alphabet_size_; ++i)
        {
            int next_state = transitions_[current_state][i];
            if (!reachable_flags[next_state])
            {
                MarkReachableStates(next_state, reachable_flags);
            }
        }
    }

    ReversedFSM GetReversedTransitions() const
    {
        ReversedFSM reversed = ReversedFSM(states_number_,
            std::vector<std::set<int> >(alphabet_size_));

        for (int current_state = 0; current_state < states_number_; ++current_state)
        {
            for (int symbol = 0; symbol < alphabet_size_; ++symbol)
            {
                int next_state = transitions_[current_state][symbol];
                reversed[next_state][symbol].insert(current_state);
            }
        }
        return reversed;
    }

    void MarkNotEquivalentStates(int first, int second, MatrixBool &not_equivalent_states,
        const ReversedFSM &reversed) const
    {
        not_equivalent_states[first][second] = true;

        for (int symbol = 0; symbol < alphabet_size_; ++symbol)
        {
            for (auto first_it = reversed[first][symbol].begin();
                first_it != reversed[first][symbol].end();
                ++first_it)
            {
                for (auto second_it = reversed[second][symbol].begin();
                    second_it != reversed[second][symbol].end();
                    ++second_it)
                {
                    if (!not_equivalent_states[*first_it][*second_it])
                    {
                        MarkNotEquivalentStates(*first_it, *second_it, 
                                                not_equivalent_states, reversed);
                    }
                }
            }
        }
    }

    std::vector<bool> GetReachableFlags() const
    {
        std::vector<bool> reachable_flags = std::vector<bool>(transitions_.size(), false);
        MarkReachableStates(kStartState, reachable_flags);
        return reachable_flags;
    }

    MatrixBool GetMatrixOfStateEquivalence(const std::vector<bool> &reachable) const
    {
        ReversedFSM reversed_transitions = GetReversedTransitions();
        MatrixBool not_equivalent_states =
            MatrixBool(states_number_, std::vector<bool>(states_number_, false));

        for (int i = 0; i < states_number_; ++i)
        {
            for (int j = 0; j < states_number_; ++j)
            {
                bool is_terminal_i = terminal_states_.count(i);
                bool is_terminal_j = terminal_states_.count(j);
                if (is_terminal_i ^ is_terminal_j)
                {
                    MarkNotEquivalentStates(i, j, not_equivalent_states, reversed_transitions);
                }
            }
        }
        return not_equivalent_states;
    }

public:
    FiniteStateMachine(const Matrix &transitions, const std::set<int> &terminal_states) :
        transitions_(transitions),
        terminal_states_(terminal_states)
    {
        states_number_ = transitions_.size();
        alphabet_size_ = (states_number_ != 0) ? transitions_[0].size() : 0;
    }    

    int GetMinStatesNumber() const
    {
        std::vector<bool> is_reachable = GetReachableFlags();  
        MatrixBool not_equivalent_states = GetMatrixOfStateEquivalence(is_reachable);

        std::vector<int> result_states;
        for (int state = 0; state < states_number_; ++state)
        {
            if (is_reachable[state])
            {
                bool has_equivalent = false;
                for (int i = 0; i < result_states.size(); ++i)
                {
                    if (result_states[i] != state && 
                        not_equivalent_states[state][result_states[i]] == 0)
                    {
                        has_equivalent = true;
                        break;
                    }
                }
                if (!has_equivalent)
                {
                    result_states.push_back(state);
                }
            }
        }
        return result_states.size();
    }
};

int GetSymbolIndex(char symbol)
{
    const char kFirstSymbol = 'a';
    return symbol - static_cast<int>(kFirstSymbol);
}

FiniteStateMachine ReadFiniteStateMachine()
{
    int states_number = 0;
    std::cin >> states_number;
    int terminal_states_number;
    std::cin >> terminal_states_number;
    int alphabet_size = 0;
    std::cin >> alphabet_size;

    std::set<int> terminal_states;
    for (int i = 0; i < terminal_states_number; ++i)
    {
        int terminal_state = -1;
        std::cin >> terminal_state;
        terminal_states.insert(terminal_state);
    }

    Matrix transitions = Matrix(states_number, std::vector<int>(alphabet_size, 0));
    int transitions_number = states_number * alphabet_size;

    for (int i = 0; i < transitions_number; ++i)
    {
        int current_state = 0;
        std::cin >> current_state;
        char transition_symbol = 0;
        std::cin >> transition_symbol;
        int next_state = 0;
        std::cin >> next_state;
        transitions[current_state][GetSymbolIndex(transition_symbol)] = next_state;
    }
    return FiniteStateMachine(transitions, terminal_states);
}

int main()
{
    FiniteStateMachine fsm = ReadFiniteStateMachine();
    std::cout << fsm.GetMinStatesNumber() << std::endl;

    return 0;
}
