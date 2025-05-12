#include "Model/Graph.h"
#include <iostream>


void PrintFaces(const std::vector<std::vector<int>>& faces, std::ostream& output);


int main()
{
    try
    {
        Graph graph("./sand.tgf");
        graph.PrintGraph(std::cout);
        std::cout << std::endl;

        if (graph.IsPlanar())
        {
            std::cout << "Граф планарный" << std::endl;

            auto faces = Graph::FindFaces(graph.GetMatrix());
            PrintFaces(faces, std::cout);
        }
        else
        {
            std::cout << "Граф не планарный" << std::endl;
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
    return 0;
}


void PrintFaces(const std::vector<std::vector<int>>& faces, std::ostream& output)
{
    for (const auto& face : faces)
    {
        for (const auto& node : face)
        {
            output << node + 1 << ' ';
        }
        output << std::endl;
    }
}