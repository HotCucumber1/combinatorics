#include "Graph.h"
#include "catch2/catch_all.hpp"


TEST_CASE("Strong Connectivity component")
{
    SECTION("Graph from presentation")
    {
        std::vector<std::vector<bool>> matrix = {
            {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        };
        std::vector<std::vector<int>> components = {
            {11}, {12}, {13},
            {2, 4}, {7, 10, 9},
            {0, 1, 3, 5}, {8}, {6}
        };
        Graph graph = Graph();
        graph.LoadGraphFromMatrix(matrix);
        REQUIRE(components == graph.GetStrongConnectivityComponents());
    }

    SECTION("Wheel graph")
    {
        std::vector<std::vector<bool>> matrix = {
            {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0}
        };
        std::vector<std::vector<int>> components = {
            {0, 5, 4, 3, 2, 1}
        };
        Graph graph = Graph();
        graph.LoadGraphFromMatrix(matrix);
        REQUIRE(components == graph.GetStrongConnectivityComponents());
    }

    SECTION("Single vertexes graph")
    {
        std::vector<std::vector<bool>> matrix = {
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0}
        };
        std::vector<std::vector<int>> components = {
            {5}, {4}, {3}, {2}, {1}, {0}
        };
        Graph graph = Graph();
        graph.LoadGraphFromMatrix(matrix);
        REQUIRE(components == graph.GetStrongConnectivityComponents());
    }
}