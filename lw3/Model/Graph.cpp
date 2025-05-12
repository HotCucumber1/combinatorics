#include "Graph.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <queue>


struct Segment
{
    std::set<int> component;
    std::set<int> contacts;
};


Graph::Graph(const std::string& fileName)
{
    m_adjacencyMatrix = ReadFromFile(fileName);
}

size_t Graph::GetSize() const
{
    return m_adjacencyMatrix.size();
}

Graph::Matrix Graph::GetMatrix() const
{
    return m_adjacencyMatrix;
}

bool IsSegmentInFace(const Segment& segment, const std::vector<int>& face)
{
    for (const auto& node : segment.contacts)
    {
        auto it = std::find(face.begin(), face.end(), node);
        if (it == face.end())
        {
            return false;
        }
    }
    return true;
}

Graph::Matrix Graph::GetGraphFromFace(const Graph::Face& face, size_t graphSize)
{
    Graph::Matrix matrix;
    matrix.resize(graphSize, std::vector<int>(graphSize, 0));

    for (int i = 0; i < face.size(); i++)
    {
        if (i + 1 >= face.size())
        {
            matrix[face[i]][face[0]] = 1;
            matrix[face[0]][face[i]] = 1;
        }
        else
        {
            matrix[face[i]][face[i + 1]] = 1;
            matrix[face[i + 1]][face[i]] = 1;
        }
    }
    return matrix;
}

bool IsNodeInGraph(int node, const Graph::Matrix& graph)
{
    return std::find(graph[node].begin(), graph[node].end(), 1) != graph[node].end();
}


Graph::Matrix Graph::GraphDiff(const Graph::Matrix& originGraph, const Graph::Matrix& subtrahendGraph)
{
    auto newGraph = originGraph;
    for (int i = 0; i < originGraph.size(); i++)
    {
        for (int j = 0; j < originGraph[i].size(); j++)
        {
            if (subtrahendGraph[i][j] == 1)
            {
                newGraph[i][j] = 0;
            }
        }
    }
    return newGraph;
}

Graph::Matrix GraphAdd(const Graph::Matrix& originGraph, const Graph::Matrix& addingGraph)
{
    auto newGraph = originGraph;
    for (int i = 0; i < originGraph.size(); i++)
    {
        for (int j = 0; j < originGraph[i].size(); j++)
        {
            if (addingGraph[i][j] == 1)
            {
                newGraph[i][j] = 1;
            }
        }
    }
    return newGraph;
}

std::vector<Segment> GetSegments(const Graph::Matrix& originGraph, const Graph::Matrix& planeGraph)
{
    std::vector<Segment> segments;

    // First type
    for (int i = 0; i < originGraph.size(); i++)
    {
        for (int j = 0; j < originGraph[i].size(); j++)
        {
            if (originGraph[i][j] == 0)
            {
                continue;
            }
            if (IsNodeInGraph(i, planeGraph) and
                IsNodeInGraph(j, planeGraph) and
                planeGraph[i][j] == 0)
            {
                Segment segment;
                segment.contacts.insert(i);
                segment.contacts.insert(j);
                segments.push_back(segment);
            }
        }
    }
    // Second type
    auto secondSegmentGraph = Graph::GraphDiff(originGraph, planeGraph);
    Segment segment;
    bool isFound = false;
    for (int i = 0; i < secondSegmentGraph.size(); i++)
    {
        for (int j = 0; j < secondSegmentGraph[i].size(); j++)
        {
            if (secondSegmentGraph[i][j] == 0)
            {
                continue;
            }
            if (IsNodeInGraph(i, planeGraph))
            {
                segment.contacts.insert(i);
            }
            else
            {
                segment.component.insert(i);
            }
            isFound = true;
        }
    }
    if (isFound)
    {
        segments.push_back(segment);
    }
    return segments;
}

Graph::Face GetPathBetweenContacts(const Segment& segment, const Graph::Matrix& graph)
{
    for (const auto& startNode : segment.contacts)
    {
        std::queue<int> queue;
        std::unordered_map<int, int> parent;
        std::unordered_map<int, bool> visited;

        queue.push(startNode);
        parent[startNode] = -1;
        visited[startNode] = true;

        while (!queue.empty())
        {
            auto currentNode = queue.front();
            queue.pop();

            if (segment.contacts.contains(currentNode) && currentNode != startNode)
            {
                std::vector<int> path;
                for (int node = currentNode; node != -1; node = parent[node])
                {
                    path.push_back(node);
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (int neighbor = 0; neighbor < graph.size(); neighbor++)
            {

                if (graph[currentNode][neighbor] == 1 and neighbor != parent[currentNode] and visited[neighbor])
                {
                    std::vector<int> path;
                    path.push_back(neighbor);
                    for (int node = currentNode; node != -1; node = parent[node])
                    {
                        path.push_back(node);
                    }
                    std::reverse(path.begin(), path.end());
                    return path;
                }
                if (graph[currentNode][neighbor] != 0 and
                    (segment.component.contains(currentNode) or
                    segment.contacts.contains(currentNode)) and
                    !visited[neighbor])
                {
                    visited[neighbor] = true;
                    parent[neighbor] = currentNode;
                    queue.push(neighbor);
                }
            }
        }
    }

    return {};
}

bool Graph::IsPlanar()
{
    auto originGraph = m_adjacencyMatrix;
    auto face = GetFace(0);
    auto planeGraph = GetGraphFromFace(face, originGraph.size());

    bool isPlanar = true;
    while (isPlanar)
    {
        originGraph = GraphDiff(originGraph, planeGraph);

        auto segments = GetSegments(originGraph, planeGraph);

        if (segments.empty())
        {
            return true;
        }
        auto faces = GetAllFaces(planeGraph);

        // ищем Segment с минимальным количеством граней
        int minFacesNumber = std::numeric_limits<int>::max();
        Segment minSegment;
        for (const auto& segment : segments)
        {
            int currSegmentFaces = 0;
            for (const auto& planedFace : faces)
            {
                if (IsSegmentInFace(segment, planedFace))
                {
                    currSegmentFaces++;
                }
            }
            if (currSegmentFaces == 0)
            {
                return false;
            }
            if (currSegmentFaces < minFacesNumber)
            {
                minFacesNumber = currSegmentFaces;
                minSegment = segment;
            }
        }

        auto segmentPath = GetPathBetweenContacts(minSegment, originGraph);
        auto segmentPathGraph = GetGraphFromFace(segmentPath, originGraph.size());

        planeGraph = GraphAdd(planeGraph, segmentPathGraph);
    }
    return isPlanar;
}

Graph::Face Graph::GetFace(int startNode)
{
    AssertNodeInGraph(startNode);

    Face cycle;
    std::vector<bool> visited(m_adjacencyMatrix.size(), false);
    bool cycleIsFound = false;
    RecursiveDFS(visited, cycle, startNode, startNode, cycleIsFound);

    return cycle;
}

void Graph::RecursiveDFS(
    std::vector<bool>& visited,
    Face& cycle,
    int current,
    int parent,
    bool& cycleIsFound
)
{
    visited[current] = true;
    cycle.push_back(current);

    for (int next = 0; next < m_adjacencyMatrix.size(); next++)
    {
        if (m_adjacencyMatrix[current][next] == 1 &&
            visited[next] && next != parent && next == 0
        )
        {
            cycleIsFound = true;
            return;
        }
        if (m_adjacencyMatrix[current][next] == 1 && !visited[next])
        {
            RecursiveDFS(visited, cycle, next, current, cycleIsFound);
        }
        if (cycleIsFound)
        {
            return;
        }
    }
    cycle.pop_back();
}

std::vector<std::vector<int>> Graph::GetAllFaces(const Graph::Matrix& matrix)
{
    std::vector<std::vector<int>> faces;
    std::vector<bool> visited(matrix.size(), false);
    std::vector<int> path;

    for (int i = 0; i < matrix.size(); i++)
    {
        if (visited[i])
        {
            continue;
        }
        FindFacesDFS(matrix, i, -1, visited, path, faces);
    }
    return faces;
}

void Graph::FindFacesDFS(
    const Matrix& graph,
    int vertex,
    int parent,
    std::vector<bool>& visited,
    std::vector<int>& path,
    std::vector<std::vector<int>>& faces
) {
    visited[vertex] = true;
    path.push_back(vertex);

    for (int i = 0; i < graph.size(); i++)
    {
        if (graph[vertex][i] == 0 or i == parent)
        {
            continue;
        }
        if (!visited[i])
        {
            FindFacesDFS(graph, i, vertex, visited, path, faces);
        }
        auto it = find(path.begin(), path.end(), i);

        if (it != path.end())
        {
            std::vector<int> face(it, path.end());
            faces.push_back(face);
        }
    }
    path.pop_back();
}


Graph::Matrix Graph::ReadFromFile(const std::string& fileName)
{
    Matrix matrix;

    std::ifstream inputFile(fileName);
    AssertStreamIsOpen(inputFile);
    auto graphSize = ReadGraphSize(inputFile);

    matrix.resize(graphSize, std::vector<int>(graphSize, 0));
    ReadEdges(inputFile, matrix);

    return matrix;
}

void Graph::PrintGraph(std::ostream &output)
{
    for (const auto& row : m_adjacencyMatrix)
    {
        for (const auto node : row)
        {
            output << node << ' ';
        }
        output << std::endl;
    }
}

int Graph::ReadGraphSize(std::istream &input)
{
    const std::string endOfReading = "#";
    std::string line;

    int nodesCount = 0;
    while (true)
    {
        std::getline(input, line);
        if (line == endOfReading)
        {
            break;
        }
        std::istringstream iss(line);
        nodesCount++;
    }
    return nodesCount;
}

void Graph::ReadEdges(
    std::istream& input,
    Graph::Matrix& matrix
)
{
    std::string line;
    while (std::getline(input, line))
    {
        std::istringstream iss(line);

        int firstNode;
        int secondNode;
        iss >> firstNode >> secondNode;
        matrix[firstNode - 1][secondNode - 1] = 1;
    }
}

void Graph::AssertStreamIsOpen(std::istream &input)
{
    if (input.fail())
    {
        throw std::runtime_error("Wrong input");
    }
}

void Graph::AssertNodeInGraph(int node)
{
    if (node >= m_adjacencyMatrix.size())
    {
        throw std::runtime_error("Wrong start node");
    }
}

Graph::Matrix Graph::FindFaces(const Matrix& matrix)
{
    size_t matrixSize = matrix.size();
    std::set<std::pair<int, int>> edges;
    std::vector<std::vector<int>> neighbors(matrixSize);

    for (int node = 0; node < matrixSize; node++)
    {
        for (int neighbour = 0; neighbour < matrixSize; neighbour++)
        {
            if (matrix[node][neighbour])
            {
                edges.insert({node, neighbour});
                neighbors[node].push_back(neighbour);
            }
        }
    }

    for (int i = 0; i < matrixSize; i++)
    {
        std::sort(neighbors[i].begin(), neighbors[i].end());
    }

    std::set<std::pair<int, int>> visited;
    std::vector<std::vector<int>> faces;

    auto next_neighbor = [&](int from, int at) {
        const auto& neighbour = neighbors[at];
        auto it = find(neighbour.begin(), neighbour.end(), from);
        if (it == neighbour.end())
        {
            return -1;
        }
        size_t nextNeighbourIndex = (it - neighbour.begin() + 1) % neighbour.size();
        return neighbour[nextNeighbourIndex];
    };

    for (const auto& [node, nextNode] : edges)
    {
        if (visited.count({node, nextNode}))
        {
            continue;
        }

        std::vector<int> face;
        int startNode = node;
        int startNextNode = nextNode;
        face.push_back(node);

        int currentNode = node;
        int currentNextNode = nextNode;

        while (true)
        {
            visited.insert({currentNode, currentNextNode});
            if (currentNextNode != face[0])
            {
                face.push_back(currentNextNode);
            }

            int next = next_neighbor(currentNode, currentNextNode);
            if (next == -1)
            {
                break;
            }
            currentNode = currentNextNode;
            currentNextNode = next;

            if (currentNode == startNode && currentNextNode == startNextNode)
            {
                break;
            }
        }
        if (face.size() >= 3)
        {
            faces.push_back(face);
        }
    }

    return faces;
}