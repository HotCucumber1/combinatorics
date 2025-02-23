#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <vector>
#include <iterator>
#include <limits>
#include <chrono>


const double NANO_IN_SECOND = 1000000000;
const long YEARS = NANO_IN_SECOND * 3600 * 24 * 365;


void assertInputValid(int fileCount);
std::ifstream openFile(const std::string& fileName);
std::vector<std::vector<int>> getMatrix(std::ifstream& inputFile);
void assertMatrixSizesAreEqual(std::vector<std::vector<int>> matrix1, std::vector<std::vector<int>> matrix2);
void printMinPath( std::vector<std::vector<int>> distances, std::vector<std::vector<int>> amounts);
void exitWithMessage(const std::string& message);
double calculateTime(double currentTime, int iterations, int currentSize);


int main(int argc, char* args[])
{
    assertInputValid(argc);

    auto distanceFile = openFile(args[1]);
    auto amountFile = openFile(args[2]);

    auto distances = getMatrix(distanceFile);
    auto amounts = getMatrix(amountFile);
    assertMatrixSizesAreEqual(distances, amounts);

    auto startTime = std::chrono::high_resolution_clock::now();
    printMinPath(distances, amounts);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = (endTime - startTime).count();

    std::cout << "Время выполнения: " << time / NANO_IN_SECOND << " секунд" << std::endl << std::endl;
    std::cout << "Время выполнения для 20 элементов: "
              << calculateTime(time, 20, distances.size())<< " лет"<< std::endl;
    std::cout << "Время выполнения для 50 элементов: "
              << calculateTime(time, 50, distances.size()) << " лет" << std::endl;
}


void exitWithMessage(const std::string& message)
{
    std::cout << message << std::endl;
    exit(1);
}

void assertInputValid(int fileCount)
{
    const int VALID_FILE_COUNT = 3;

    if (fileCount != VALID_FILE_COUNT) {
        exitWithMessage("Передано неверное количество файлов");
    }
}

void assertFileValid(std::ifstream& file)
{
    if (!file.is_open()) {
        exitWithMessage("Ошибка открытия файла");
    }
}

void assertMatrixSizesAreEqual(
    std::vector<std::vector<int>> matrix1,
    std::vector<std::vector<int>> matrix2
)
{
    bool rowsCountIsEqual = matrix1.size() == matrix2.size();
    bool columnsCountIsEqual = matrix1[0].size() == matrix2[0].size();

    if (!rowsCountIsEqual || !columnsCountIsEqual) {
        exitWithMessage("Количество элементов в матрицах различно");
    }
}

std::ifstream openFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    assertFileValid(file);
    return file;
}

std::vector<std::vector<int>> getMatrix(std::ifstream& inputFile)
{
    std::vector<std::vector<int>> matrix;
    std::string line;

    std::getline(inputFile, line);
    std::istringstream iss(line);
    int num;

    std::vector<int> firstRow;
    while (iss >> num) {
        firstRow.push_back(num);
    }
    matrix.push_back(firstRow);

    int matrixSize = firstRow.size();

    for (int i = 1; i < matrixSize; i++) {
        std::getline(inputFile, line);
        std::istringstream iss(line);

        std::vector<int> row;
        for (int j = 0; j < matrixSize; j++) {
            iss >> num;
            row.push_back(num);
        }
        matrix.push_back(row);
    }
    return matrix;
}

void printMinPath(
    std::vector<std::vector<int>> distances,
    std::vector<std::vector<int>> amounts
)
{
    int minPath = std::numeric_limits<int>::max();
    std::vector<int> path;

    std::vector<int> factories(distances.size());
    std::iota(factories.begin(), factories.end(), 0);
    std::sort(factories.begin(), factories.end());

    do
    {
        int currentPath = 0;

        for (int i = 0; i < distances.size(); i++) {
            for (int j = 0; j < distances[i].size(); j++) {
                currentPath += distances[i][j] * amounts[factories[i]][factories[j]];
            }
        }
        if (currentPath < minPath) {
            minPath = currentPath;
            path = factories;
        }
    } while (std::next_permutation(factories.begin(), factories.end()));

    std::cout << "Способ расстановки: ";
    copy(
        path.begin(),
        path.end(),
        std::ostream_iterator<int>(std::cout, " ")
    );
    std::cout << std::endl << std::endl;
    std::cout << "Минимальный поток: " << minPath << std::endl << std::endl;
}

double calculateTime(
    double currentTime,
    int iterations,
    int currentSize
)
{
    currentTime /= YEARS;
    for (int i = currentSize + 1; i <= iterations; i++) {
        currentTime *= i;
    }
    return currentTime;
}