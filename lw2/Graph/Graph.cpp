#include "Graph.h"
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <stack>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <utility>


Graph::Graph() = default;

void Graph::PrintAdjacencyMatrix(std::ostream &output)
{
    if (m_matrixSize == 0)
    {
        output << "Graph is not loaded" << std::endl;
        return;
    }
    for (auto & currentNode : m_adjacencyMatrix)
    {
        for (auto && isAchievable : currentNode)
        {
            output << isAchievable << ' ';
        }
        output << std::endl;
    }
}

void Graph::PrintEdges(std::ostream &output)
{
    if (m_matrixSize == 0)
    {
        output << "Graph is not loaded" << std::endl;
        return;
    }
    output << m_matrixSize << std::endl;
    for (size_t currentNode = 0; currentNode < m_adjacencyMatrix.size(); currentNode++)
    {
        for (size_t nextNode = 0; nextNode < m_adjacencyMatrix[currentNode].size(); nextNode++)
        {
            if (!m_adjacencyMatrix[currentNode][nextNode])
            {
                continue;
            }
            output << currentNode<< ' ' << nextNode << std::endl;
        }
    }
}

void Graph::DFS(const std::vector<int>& nodes)
{
    m_treeEdges.clear();
    std::vector<bool> visited(m_matrixSize, false);
    std::stack<int> stack;
    int time = 1;

    m_strongConnectivityComponents.clear();
    for (const auto& startPoint : nodes)
    // for (int startPoint = 0; startPoint < m_matrixSize; startPoint++)
    {
        std::vector<int> currentComponent = {startPoint};
        if (visited[startPoint])
        {
            continue;
        }
        stack.push(startPoint);
        visited[startPoint] = true;
        m_timePoints[startPoint].inTime = time++;

        while (!stack.empty())
        {
            int currentVertex = stack.top();
            bool hasUnvisitedNeighbor = false;

            for (int i = 0; i < m_matrixSize; ++i)
            {
                if (!m_adjacencyMatrix[currentVertex][i] || visited[i])
                {
                    continue;
                }
                stack.push(i);
                visited[i] = true;
                m_timePoints[i].inTime = time++;
                hasUnvisitedNeighbor = true;
                m_treeEdges.emplace_back(currentVertex, i);
                currentComponent.push_back(i);
                break;
            }
            if (hasUnvisitedNeighbor)
            {
                continue;
            }
            stack.pop();
            m_timePoints[currentVertex].outTime = time++;
        }
        m_strongConnectivityComponents.push_back(currentComponent);
    }
}

std::vector<std::vector<int>> Graph::GetStrongConnectivityComponents()
{
    auto nodes = GetNodesInDecreaseOutTimeOrder();
    InvertGraph();
    DFS(nodes);
    auto components = m_strongConnectivityComponents;
    InvertGraph();
    return components;
}

std::vector<int> Graph::GetNodesInDecreaseOutTimeOrder()
{
    std::vector<int> invertedNodes = m_nodes;
    std::ranges::sort(invertedNodes, [&](const int firstNode, const int secondNode){
        return m_timePoints[firstNode].outTime > m_timePoints[secondNode].outTime;
    });
    return invertedNodes;
}

void Graph::PrintTimePoints(std::ostream& output)
{
    if (m_timePoints.empty())
    {
        output << "Graph is not loaded" << std::endl;
        return;
    }
    for (const auto& [node, timePoint] : m_timePoints)
    {
        output << node << " [" << timePoint.inTime << ", " << timePoint.outTime << ']' << std::endl;
    }
}

void Graph::PrintTreeEdges(std::ostream& output)
{
    for (const auto& [inVertex, outVertex] : m_treeEdges)
    {
        output << inVertex << " --> " << outVertex << std::endl;
    }
}

void Graph::LoadGraphFromMatrix(const std::vector<std::vector<bool>>& matrix)
{
    m_adjacencyMatrix = matrix;
    m_matrixSize = matrix.size();
    m_nodes.resize(m_matrixSize);
    std::iota(m_nodes.begin(), m_nodes.end(), 0);
    DFS(m_nodes);
}

std::vector<std::vector<bool>> Graph::GetAdjacencyMatrix()
{
    return m_adjacencyMatrix;
}

void Graph::ReadDataFromFile(const std::string& fileName)
{
    std::ifstream inputFile(fileName);
    AssertFileIsOpen(inputFile);

    ReadGraphSize(inputFile);
    m_adjacencyMatrix.resize(m_matrixSize, std::vector<bool>(m_matrixSize, false));
    ReadGraphEdges(inputFile);
    DFS(m_nodes);
}

void Graph::ReadGraphSize(std::istream &inputFile)
{
    if (!(inputFile >> m_matrixSize))
    {
        throw std::runtime_error("Need to specify graph size");
    }
    m_nodes.resize(m_matrixSize);
    std::iota(m_nodes.begin(), m_nodes.end(), 0);
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Graph::ReadGraphEdges(std::istream &inputFile)
{
    std::string line;
    while (std::getline(inputFile, line))
    {
        AsserEdgeIsCorrect(line);
        std::istringstream iss(line);

        int firstNode;
        int secondNode;
        iss >> firstNode >> secondNode;
        m_adjacencyMatrix[firstNode - 1][secondNode - 1] = true;
    }
}

void Graph::InvertGraph()
{
    std::vector<std::vector<bool>> invertedGraph;
    invertedGraph.resize(m_matrixSize, std::vector<bool>(m_matrixSize, false));
    for (int i = 0; i < m_matrixSize; i++)
    {
        for (int j = 0; j < invertedGraph[i].size(); j++)
        {
            invertedGraph[i][j] = m_adjacencyMatrix[j][i];
        }
    }
    m_adjacencyMatrix = invertedGraph;
    DFS(m_nodes);
}

void Graph::AssertFileIsOpen(std::istream &inputFile)
{
    if (inputFile.fail())
    {
        throw std::ifstream::failure("Failed to open file");
    }
}

void Graph::AsserEdgeIsCorrect(const std::string &edge)
{
    std::istringstream iss(edge);
    int firstNode;
    int secondNode;

    if (!(iss >> firstNode) || !(iss >> secondNode))
    {
        throw std::runtime_error("Wrong edge list");
    }
}
