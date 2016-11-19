#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

const int INFINITY_VALUE = 1e9;
const char NO_SOLUTION[] = "No solution";

struct Coin
{
    int coordinate;
    int lifetime;
};

bool operator < (Coin first, Coin second)
{
    return first.coordinate < second.coordinate;
}

int Distance(Coin first_coin, Coin second_coin)
{
    return std::abs(first_coin.coordinate - second_coin.coordinate);
}

class CollectingTimeCounter
{
    typedef vector< vector<int> > Table;

public:

    explicit CollectingTimeCounter(vector<Coin> &coins) : coins_(coins)
    {
        int coins_number = coins_.size();
        collecting_time_table_ = Table(coins_number, vector<int>(coins_number));
    }

    int CountCollectingTime()
    {
        std::sort(coins_.begin(), coins_.end());
        int coins_number = collecting_time_table_.size();
        for (int diagonal = 1; diagonal < coins_number; ++diagonal)
        {
            for (int line = 0; line < coins_number - diagonal; ++line)
            {
                int left_coin = line;
                int right_coin = line + diagonal;
                RecountCollectingTime(left_coin, right_coin, right_coin - 1);
                RecountCollectingTime(right_coin, left_coin, left_coin + 1);
            }
        }
        return GetMinimalCollectingTime();
    }

private:

    vector<Coin> &coins_;
    Table collecting_time_table_;

    void RecountCollectingTime(int left_coin, int right_coin, int previous_coin)
    {
        int time_forward = collecting_time_table_[left_coin][previous_coin] +
            Distance(coins_[right_coin], coins_[previous_coin]);

        int time_backward = collecting_time_table_[previous_coin][left_coin] +
            Distance(coins_[right_coin], coins_[left_coin]);

        int result_time = std::min(time_forward, time_backward);

        collecting_time_table_[left_coin][right_coin] =
            (result_time <= coins_[right_coin].lifetime) ? result_time : INFINITY_VALUE;
    }

    int GetMinimalCollectingTime() const
    {
        int coins_number = coins_.size();
        return std::min(collecting_time_table_[0][coins_number - 1],
            collecting_time_table_[coins_number - 1][0]);
    }

};

vector<Coin> ReadCoins(int coins_number)
{
    std::vector<Coin> coins(coins_number);
    for (int i = 0; i < coins_number; ++i)
    {
        std::cin >> coins[i].coordinate >> coins[i].lifetime;
    }
    return coins;
}

void PrintResult(long long collecting_time)
{
    if (collecting_time == INFINITY_VALUE)
    {
        std::cout << NO_SOLUTION << std::endl;
    }
    else
    {
        std::cout << collecting_time << std::endl;
    }
}

int main()
{
    int coins_number = 0;
    std::cin >> coins_number;
    vector<Coin> coins = ReadCoins(coins_number);

    int collecting_time = CollectingTimeCounter(coins).CountCollectingTime();
    PrintResult(collecting_time);

    return 0;
}