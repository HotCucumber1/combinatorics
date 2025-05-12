#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <set>

class Graph
{
public:
    using Face = std::vector<int>;
    using Matrix = std::vector<std::vector<int>>;
public:
    explicit Graph(const std::string& fileName);
    Face GetFace(int startNode);
    size_t GetSize() const;
    bool IsPlanar();
    Matrix GetMatrix() const;
    void PrintGraph(std::ostream& output);
    static Matrix FindFaces(const Matrix& matrix);
    static Matrix GetGraphFromFace(const Face& face, size_t graphSize);
    static Matrix GraphDiff(const Matrix& originGraph, const Matrix& subtrahendGraph);

private:
    using Edge = std::pair<int, int>;
    Matrix m_adjacencyMatrix;

private:
    void RecursiveDFS(
        std::vector<bool>& visited,
        std::vector<int>& cycle,
        int current,
        int parent,
        bool& cycleIsFound
    );
    void FindFacesDFS(
        const Matrix& graph,
        int vertex,
        int parent,
        std::vector<bool>& visited,
        std::vector<int>& path,
        std::vector<std::vector<int>>& faces
    );
    std::vector<std::vector<int>> GetAllFaces(const Graph::Matrix& matrix);
    void AssertNodeInGraph(int node);

    static int ReadGraphSize(std::istream& input);
    static void ReadEdges(std::istream& input, Matrix& matrix);
    static void AssertStreamIsOpen(std::istream& input);
    static Matrix ReadFromFile(const std::string& fileName);
};

// TODO: граф задан уточнить
// TODO: бак
// TODO: разряженный граф
// TODO: почему квазилинейно
// TODO:
