// #define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <stack>
using std::vector;

struct elem
{
    int val;
    int index;
};

typedef elem Elem;

vector<int> readArray(int size)
{
    vector<int> arr = vector<int>(size);
    for (int i = 0; i < size; ++i)
    {
        std::cin >> arr[i];
    }
    return arr;
}   

int Max(int one, int another)
{
    return (one > another) ? one : another;
}

Elem Recount(const vector<Elem> &length, int first_index, int second_index)
{
    Elem res;
    if (length[first_index].val == length[second_index].val + 1)
    {
       return length[first_index];
    }

    if (length[first_index].val > length[second_index].val + 1)
    {
        res.val = length[first_index].val;
        res.index = first_index;
    }
    else
    {
        res.val = length[second_index].val + 1;
        res.index = second_index;
    }
    return res;
}

std::stack<int> GetSequence(const vector<int> &arr, const vector<Elem> &length, int max_index)
{
    int size = length.size() - 1;
    int max = length[max_index].val;
    std::stack<int> sequence = std::stack<int>();
    int index = max_index;
    while (index >= 0 && length[index].val == max )
    {
        index--;
    }
    sequence.push(arr[index + 1]);
    index++;
    while (index >= 1)
    {
        sequence.push(arr[length[index].index]);
        index = length[index].index;
    }
    return sequence;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int size = 0;
    std::cin >> size;
    vector<int> arr = readArray(size);
    vector<Elem> up = vector<Elem>(size);
    vector<Elem> down = vector<Elem>(size);

    for (int index_i = 0; index_i < size; ++index_i)
    {
        up[index_i].val = 1;
        up[index_i].index = -1;
        down[index_i].val = 1;
        down[index_i].index = -1;
        for (int index_j = 0; index_j < index_i; ++index_j)
        {
            if (up[index_j].val % 2 == 0)
            {
                if (arr[index_i] > arr[index_j])
                {
                    up[index_i] = Recount(up, index_i, index_j);
                }
            }
            else
            {
                if (arr[index_i] < arr[index_j])
                {
                    up[index_i] = Recount(up, index_i, index_j);
                }
            }
            if (down[index_j].val % 2 == 0)
            {
                if (arr[index_i] < arr[index_j])
                {
                    down[index_i] = Recount(down, index_i, index_j);
                }
            }
            else
            {
                if (arr[index_i] > arr[index_j])
                {
                    down[index_i] = Recount(down, index_i, index_j);
                }
            }
        }
    }

    int max_index = -1;
    Elem max;
    max.val = 0;

    for (int index = 0; index < size; ++index)
    {
        if (up[index].val > max.val)
        {
            max_index = 0;
            max.val = up[index].val;
            max.index = index;
        }
        if (down[index].val > max.val)
        {
            max_index = 1;
            max.val = down[index].val;
            max.index = index;
        }        
    }

    std::stack<int> sequence;
    if (max_index == 1)
    {
        sequence = GetSequence(arr, down, max.index);
    }

    else
    {
        sequence = GetSequence(arr, up, max.index);
    }
    
    while (!sequence.empty())
    {
        std::cout << sequence.top() << " ";
        sequence.pop();
    }
    return 0;
}
