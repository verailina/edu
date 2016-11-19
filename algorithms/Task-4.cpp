#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

class Treap
{
private:

    struct Node
    {
        int key_;
        int priority_;
        Node *left_son_;
        Node *right_son_;

        Node(int key, int priority)
        {
            key_ = key;
            priority_ = priority;
            left_son_ = NULL;
            right_son_ = NULL;
        }
    };

    Node *root_;

    static Node *Join(Node *first, Node *second)
    {
        if (first == NULL)
        {
            return second;
        }

        if (second == NULL)
        {
            return first;
        }

        if (first->priority_ <= second->priority_)
        {
            first->right_son_ = Join(first->right_son_, second);
            return first;
        }
        else
        {
            second->left_son_ = Join(first, second->left_son_);
            return second;
        }
    }

    static void TriSplit(Node *root, int key, Node **left_root, Node **right_root, Node **key_node)
    {
        if (root == NULL)
        {
            *left_root = NULL;
            *right_root = NULL;
            *key_node = NULL;
            return;
        }

        if (key == root->key_)
        {
            *left_root = root->left_son_;
            *right_root = root->right_son_;
            root->left_son_ = NULL;
            root->right_son_ = NULL;
            *key_node = root;
            return;
        }

        if (key < root->key_)
        {
            Node *middle_tree = NULL;
            TriSplit(root->left_son_, key, left_root, &middle_tree, key_node);
            root->left_son_ = middle_tree;
            *right_root = root;
        }

        else
        {
            Node *middle_tree = NULL;
            TriSplit(root->right_son_, key, &middle_tree, right_root, key_node);
            root->right_son_ = middle_tree;
            *left_root = root;
        }
    }

    static Node * Find(Node *root, int key)
    {
        Node *left_tree = NULL;
        Node *right_tree = NULL;
        Node *key_node = NULL;

        TriSplit(root, key, &left_tree, &right_tree, &key_node);
        root = Join(Join(left_tree, key_node), right_tree);
        return key_node;
    }

    static Node* GetLeftmostNode(Node *root)
    {
        if (root == NULL)
        {
            return NULL;
        }

        while (root->left_son_ != NULL)
        {
            root = root->left_son_;
        }
        return root;
    }

    static Node * GetNextKeyNode(Node *root, int key)
    {
        Node *left_tree = NULL;
        Node *right_tree = NULL;
        Node *key_node = NULL;

        TriSplit(root, key, &left_tree, &right_tree, &key_node);
        Node *next_key_node = GetLeftmostNode(right_tree);
        root = Join(Join(left_tree, key_node), right_tree);
        return next_key_node;
    }

    static void DeleteTree(Node *root)
    {
        if (root == NULL)
        {
            return;
        }
        DeleteTree(root->left_son_);
        DeleteTree(root->right_son_);
        delete root;
    }

public:

    Treap()
    {
        root_ = NULL;
    }

    bool IsEmpty() const
    {
        return root_ == NULL;
    }

    bool Insert(int key)
    {
        int priority = rand();
        Node *new_node = new Node(key, priority);
        Node *left_tree = NULL;
        Node *right_tree = NULL;
        Node *node = NULL;

        TriSplit(root_, key, &left_tree, &right_tree, &node);
        if (node != NULL)
        {
            return false;
        }

        root_ = Join(Join(left_tree, new_node), right_tree);
        return true;
    }

    void Remove(int key)
    {
        Node *left_tree = NULL;
        Node *right_tree = NULL;
        Node *key_node = NULL;

        TriSplit(root_, key, &left_tree, &right_tree, &key_node);
        root_ = Join(left_tree, right_tree);
        DeleteTree(key_node);
    }

    bool ContainsKey(int key) const
    {
        Node *key_node = Find(root_, key);
        return key_node != NULL;
    }

    bool GetMinKey(int *min_key) const
    {
        Node *min_key_node = GetLeftmostNode(root_);
        if (min_key_node == NULL)
        {
            return false;
        }
        *min_key = min_key_node->key_;
        return true;
    }

    bool GetNextKey(int key, int *next_key) const
    {
        Node *next_key_node = GetNextKeyNode(root_, key);
        if (next_key_node == NULL)
        {
            return false;
        }
        *next_key = next_key_node->key_;
        return true;
    }

    ~Treap()
    {
        DeleteTree(root_);
    }
};

struct QueryResult
{
    enum ResultType
    {
        POSITION_OF_CAR,
        PARKING_IS_FULL,
        POSITION_IS_FREE,
        POSITON_WAS_OCCUPIED
    };

    ResultType result_type_;
    int position_;

    explicit QueryResult(int position)
    {
        position_ = position;
        result_type_ = POSITION_OF_CAR;
    }

    explicit QueryResult(ResultType type)
    {
        result_type_ = type;
    }
};



void PrintQueryResult(const QueryResult &result)
{
    const int kParkingIsFull = -1;
    const int kPositionIsFree = -2;
    const int kPositionWasOccupied = 0;

    switch (result.result_type_)
    {
    case QueryResult::PARKING_IS_FULL:
    {
        std::cout << kParkingIsFull << std::endl;
        break;
    }
    case QueryResult::POSITION_OF_CAR:
    {
        std::cout << result.position_ + 1 << std::endl;
        break;
    }
    case QueryResult::POSITION_IS_FREE:
    {
        std::cout << kPositionIsFree << std::endl;
        break;
    }
    case QueryResult::POSITON_WAS_OCCUPIED:
    {
        std::cout << kPositionWasOccupied << std::endl;
        break;
    }
    }
}

class ParkingProcessor
{
private:

    Treap free_positions_;

public:

    explicit ParkingProcessor(int size)
    {
        for (int i = 0; i < size; ++i)
        {
            free_positions_.Insert(i);
        }
    }

    QueryResult ProcessCarComingQuery(int expected_position)
    {
        if (free_positions_.IsEmpty())
        {
            return QueryResult(QueryResult::PARKING_IS_FULL);
        }

        if (free_positions_.ContainsKey(expected_position))
        {
            free_positions_.Remove(expected_position);
            return QueryResult(expected_position);
        }

        int next_free_position = 0;
        if (!free_positions_.GetNextKey(expected_position, &next_free_position))
        {
            if (!free_positions_.GetMinKey(&next_free_position))
            {
                return QueryResult(QueryResult::PARKING_IS_FULL);
            }
        }

        free_positions_.Remove(next_free_position);
        return QueryResult(next_free_position);
    }

    QueryResult ProcessCarLeavingQuery(int position)
    {
        if (free_positions_.ContainsKey(position))
        {
            return QueryResult(QueryResult::POSITION_IS_FREE);
        }

        free_positions_.Insert(position);
        return QueryResult(QueryResult::POSITON_WAS_OCCUPIED);
    }
};



int ReadQuery()
{
    const char kMinusSymbol = '-';

    int query = 0;
    char sign = 0;
    std::cin >> sign;
    std::cin >> query;

    if (sign == kMinusSymbol)
    {
        query *= -1;
    }
    return query;
}

const int kSeed = 123;

void ProcessInputQueries()
{
    srand(kSeed);

    int parking_size = 0;
    std::cin >> parking_size;
    int queries_number = 0;
    std::cin >> queries_number;

    ParkingProcessor processor(parking_size);

    for (int i = 0; i < queries_number; ++i)
    {
        int current_query = ReadQuery();
        if (current_query > 0)
        {
            PrintQueryResult(processor.ProcessCarComingQuery(current_query - 1));
        }
        else
        {
            PrintQueryResult(processor.ProcessCarLeavingQuery(-current_query - 1));
        }
    }
}

int main()
{
    ProcessInputQueries();
    return 0;
}
