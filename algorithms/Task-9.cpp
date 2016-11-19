#include <deque>
#include <vector>
#include <iostream>
#include <utility>

class Graph
{
private: 
    struct Edge
    {
        int length_;
        int end_vertex_;

        Edge(int end_vertex, int length) :
            length_(length), end_vertex_(end_vertex)
        {
        }
    };
    static const int kUnreachable = -1;
    std::vector<std::vector<Edge> > edges_;
    int vertices_number_;
    int edges_number_;

public:
    Graph(int vertices_number, int edges_number):
        vertices_number_(vertices_number),
        edges_number_(edges_number)
    {
        edges_ = std::vector<std::vector<Edge> >(vertices_number);
    }

    void AddEdge(int first_vertex, int second_vertex, int edge_length)
    {
        edges_[first_vertex].push_back(Edge(second_vertex, edge_length));
        edges_[second_vertex].push_back(Edge(first_vertex, edge_length));
    }

    std::vector<int> CalculateShortestDistance(int start_vertex)
    {
        std::vector<int> distance = std::vector<int>(vertices_number_, kUnreachable);
        distance[start_vertex] = 0;
        std::deque<int> vertices_to_process;
        vertices_to_process.push_back(start_vertex);
        while (!vertices_to_process.empty())
        {
            int current_vertex = vertices_to_process.front();
            vertices_to_process.pop_front();
            for (int edge_number = 0; edge_number < edges_[current_vertex].size(); ++edge_number)
            {
                int destination_vertex = edges_[current_vertex][edge_number].end_vertex_;
                int edge_length = edges_[current_vertex][edge_number].length_;
                int new_distance = (distance[current_vertex] != kUnreachable) ? 
                                    distance[current_vertex] + edge_length :
                                    edge_length;

                if (new_distance < distance[destination_vertex] ||
                    distance[destination_vertex] == kUnreachable)
                {
                    distance[destination_vertex] = new_distance;
                    if (edge_length == 0)
                    {
                        vertices_to_process.push_front(destination_vertex);
                    }
                    else
                    {
                        vertices_to_process.push_back(destination_vertex);
                    }
                }
            }
        }
        return distance;
    }

    int GetShortestDistance(int start, int end)
    {
        std::vector<int> distance = CalculateShortestDistance(start);
        int shortest_path_length = distance[end];
        return shortest_path_length;
    }
};

Graph ReadGraph(int vertices_number, int edges_number)
{
    Graph graph(vertices_number, edges_number);
    for (int edge = 0; edge < edges_number; ++edge)
    {
        int first_vertex = 0;
        int second_vecrtex = 0;
        int edge_length;
        std::cin >> first_vertex;
        std::cin >> second_vecrtex;
        std::cin >> edge_length;
        graph.AddEdge(first_vertex - 1, second_vecrtex - 1, edge_length);
    }
    return graph;
}

int main()
{
    int vertices_number = 0;
    int edges_number = 0;
    int start_vertex = 0;
    int end_vertex = 0;

    std::cin >> vertices_number;
    std::cin >> edges_number;
    std::cin >> start_vertex;
    std::cin >> end_vertex;

    Graph graph = ReadGraph(vertices_number, edges_number);
    std::cout << graph.GetShortestDistance(start_vertex - 1, end_vertex - 1);

    return 0;
}
