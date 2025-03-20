#include "Graph/Graph.h"
#include <stdexcept>
#include <algorithm>

void AssertArgumentsNumber(int argc);
void PrintComponents(const std::vector<std::vector<int>>& components);

int main(int argc, char* argv[])
{
    try
    {
        AssertArgumentsNumber(argc);
        Graph graph = Graph();
        graph.ReadDataFromFile(argv[1]);

        auto components =
            graph.GetStrongConnectivityComponents();
        PrintComponents(components);
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
    return 0;
}




void AssertArgumentsNumber(int argc)
{
    const int argumentsNumber = 2;
    if (argc != argumentsNumber)
    {
        throw std::invalid_argument("Need to specify graph file");
    }
}

void PrintComponents(const std::vector<std::vector<int>>& components)
{
    for (const auto& component : components)
    {
        for (const auto& node : component)
        {
            std::cout << node << ' ';
        }
        std::cout << std::endl;
    }
}