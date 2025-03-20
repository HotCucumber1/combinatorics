#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Graph
{
private:
    struct NodeTimePoint
    {
        int inTime = 1;
        int outTime = 0;
    };
    int m_matrixSize = 0;

    std::vector<std::vector<bool>> m_adjacencyMatrix;
    std::map<int, NodeTimePoint> m_timePoints;
    std::vector<std::pair<int, int>> m_treeEdges;
    std::vector<int> m_nodes;
    std::vector<std::vector<int>> m_strongConnectivityComponents;

public:
    explicit Graph();
    void ReadDataFromFile(const std::string& fileName);
    void PrintAdjacencyMatrix(std::ostream& output = std::cout);
    void PrintEdges(std::ostream& output = std::cout);
    void PrintTimePoints(std::ostream& output = std::cout);
    void PrintTreeEdges(std::ostream& output = std::cout);
    void InvertGraph();
    void LoadGraphFromMatrix(const std::vector<std::vector<bool>>& matrix);
    std::vector<int> GetNodesInDecreaseOutTimeOrder();
    std::vector<std::vector<bool>> GetAdjacencyMatrix();
    std::vector<std::vector<int>> GetStrongConnectivityComponents();

private:
    void DFS(const std::vector<int>& nodes);
    void ReadGraphSize(std::istream& inputFile);
    void ReadGraphEdges(std::istream& inputFile);
    static void AssertFileIsOpen(std::istream& inputFile);
    static void AsserEdgeIsCorrect(const std::string& edge);
};

