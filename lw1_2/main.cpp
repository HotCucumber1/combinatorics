#include <iostream>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>


using Workers = std::map<int, std::vector<int>>;

const double NANO_IN_SECOND = 1000000000;

struct InputFiles
{
    std::string workFileName;
    std::string workersFileName;
};

InputFiles GetInput(int argc, char* argv[]);
bool NextCombinations(size_t dim, std::vector<size_t>& state);
std::vector<int> GetMinWorkers(const std::string& workFileName,
                               const std::string& workersFileName);
std::vector<int> GetWorks(std::ifstream& file);
std::vector<int> GetKeys(const Workers& map);
Workers GetWorkers(std::ifstream& file);
bool IsWorkersEnough(const Workers& workersMap,
                     const std::vector<int>& works);
void PrintVector(const std::vector<int>& vector);
void AssertFileIsOpen(std::ifstream& file);
void AssertFileNumber(int argc);


int main(int argc, char* argv[])
{
    try
    {
        auto inputFiles = GetInput(argc, argv);
        auto startTime = std::chrono::high_resolution_clock::now();

        auto workers = GetMinWorkers(inputFiles.workFileName, inputFiles.workersFileName);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto time = (endTime - startTime).count();

        std::cout << "Требуемые работники: ";
        PrintVector(workers);

        std::cout << "Время выполнения: " << static_cast<double>(time) / NANO_IN_SECOND << std::endl;
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }
    return 0;
}

InputFiles GetInput(int argc, char* argv[])
{
    AssertFileNumber(argc);
    return {
        argv[1],
        argv[2]
    };
}


std::vector<int> GetMinWorkers(const std::string& workFileName,
                               const std::string& workersFileName)
{
    std::ifstream worksFile(workFileName);
    std::ifstream workersFile(workersFileName);
    AssertFileIsOpen(worksFile);
    AssertFileIsOpen(workersFile);

    auto works = GetWorks(worksFile);
    auto workersMap = GetWorkers(workersFile);

    size_t workersNumber = workersMap.size();
    for (size_t currentSize = 1; currentSize <= workersNumber; currentSize++)
    {
        std::vector<size_t> currentCombination(currentSize + 1);
        for (size_t i = 1; i <= currentSize; i++)
        {
            currentCombination[i] = i;
        }
        do
        {
            Workers currentWorkers;
            for (size_t i = 1; i <= currentSize; i++)
            {
                size_t index = currentCombination[i];
                currentWorkers[index] = workersMap[index];
            }
            if (IsWorkersEnough(currentWorkers, works))
            {
                return GetKeys(currentWorkers);
            }
        } while (NextCombinations(workersNumber, currentCombination));
    }
    return GetKeys(workersMap);
}

bool IsWorkersEnough(const Workers& workersMap,
                     const std::vector<int>& works)
{
    std::vector<int> resultWork;
    for (const auto& worker : workersMap)
    {
        resultWork.insert(resultWork.end(), worker.second.begin(), worker.second.end());
    }
    std::sort(resultWork.begin(), resultWork.end());
    auto last = std::unique(resultWork.begin(), resultWork.end());
    resultWork.erase(last, resultWork.end());

    return std::all_of(
        works.begin(),
        works.end(),
        [&](int elem) -> bool {
            return std::find(resultWork.begin(), resultWork.end(), elem) != resultWork.end();
        }
    );
}

std::vector<int> GetWorks(std::ifstream& file)
{
    std::vector<int> works;

    int number;
    while (file >> number)
    {
        works.push_back(number);
    }
    return works;
}

Workers GetWorkers(std::ifstream& file)
{
    std::map<int, std::vector<int>> workers;
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<int> data;
        int number;

        while (iss >> number)
        {
            data.push_back(number);
        }
        int worker = data[0];
        std::vector<int> works(data.begin() + 1, data.end());

        workers[worker] = works;
    }
    return workers;
}

bool NextCombinations(size_t dim, std::vector<size_t>& state) {
    size_t k = state.size() - 1;
    size_t m = k;
    while (state[m] == dim - k + m)
    {
        m = m - 1;
    }
    state[m] = state[m] + 1;
    for (size_t i = m + 1; i <= k; i++)
    {
        state[i] = state[i - 1] + 1;
    }
    return m != 0;
}

std::vector<int> GetKeys(const Workers& map)
{
    std::vector<int> keys;

    for (const auto& item : map)
    {
        keys.push_back(item.first);
    }
    return keys;
}

void PrintVector(const std::vector<int>& vector)
{
    for (const auto& item : vector)
    {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
}

void AssertFileNumber(int argc)
{
    const int fileNumber = 2;
    if (argc == fileNumber)
    {
        throw std::invalid_argument("Wrong argument number");
    }
}

void AssertFileIsOpen(std::ifstream& file)
{
    if (file.fail())
    {
        throw std::invalid_argument("Failed to open input file");
    }
}