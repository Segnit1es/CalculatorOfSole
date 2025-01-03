#include "InputSlau.h"
#include <random>
#include <iostream>
#include <cmath>

using namespace std;

bool Node::operator==(const Node& other) const {
    if (coefficients.size() != other.coefficients.size() ||
        freeTerms.size() != other.freeTerms.size()) {
        return false;
    }

    for (size_t i = 0; i < freeTerms.size(); ++i) {
        if (!CompareDoubles(freeTerms[i], other.freeTerms[i])) {
            return false;
        }
    }

    for (size_t i = 0; i < coefficients.size(); ++i) {
        for (size_t j = 0; j < coefficients[i].size(); ++j) {
            if (!CompareDoubles(coefficients[i][j], other.coefficients[i][j])) {
                return false;
            }
        }
    }

    return true;
}


bool CompareDoubles(double a, double b) {
    return fabs(a - b) < 0.0001;
}

bool IsEmpty(const Node& node) {
    return node.coefficients.empty() && node.freeTerms.empty();
}

int Node::GetMatrixSize() const {
    return coefficients.size();
}

void ClearNode(Node& node) {
    node.coefficients.clear();
    node.freeTerms.clear();
    node.time1 = -1;
    node.time2 = -1;
    node.time3 = -1;
    node.time4 = -1;
    node.solution1.clear();
    node.solution2.clear();
    node.solution3.clear();
    node.solution4.clear();
}

int Rank(vector<vector<double>> matrix) {
    int rowCount = matrix.size();
    int colCount = matrix[0].size();
    int rank = 0;

    for (int r = 0; r < rowCount; r++) {
        int leading = -1;
        for (int c = 0; c < colCount; c++) {
            if (leading == -1 && matrix[r][c] != 0) {
                leading = c;
            }
            if (leading != -1) {
                for (int i = r + 1; i < rowCount; i++) {
                    if (matrix[i][leading] != 0) {
                        double factor = matrix[i][leading] / matrix[r][leading];
                        for (int j = 0; j < colCount; j++) {
                            matrix[i][j] -= factor * matrix[r][j];
                        }
                    }
                }
                break;
            }
        }
        if (leading != -1) {
            rank++;
        }
    }

    return rank;
}

bool CheckOneSol(const Node& node) {
    int rankCoefficients = Rank(node.coefficients);
    vector<vector<double>> augmentedMatrix(node.coefficients);
    for (size_t i = 0; i < node.freeTerms.size(); ++i) {
        augmentedMatrix[i].push_back(node.freeTerms[i]);
    }
    int rankAugmented = Rank(augmentedMatrix);
    return (rankCoefficients == rankAugmented) && (rankCoefficients == node.coefficients.size());
}

void ModifyCoef(Node& node, int i, int j, double value) {
    if (i >= 0 && i < node.coefficients.size() && j >= 0 && j < node.coefficients[i].size()) {
        node.coefficients[i][j] = value;
    }
    else {
        cout << "Ошибка: индексы выходят за пределы матрицы." << endl;
    }
}

void ModifyFreeTerms(Node& node, int i, double value) {
    if (i >= 0 && i < node.freeTerms.size()) {
        node.freeTerms[i] = value;
    }
    else {
        cout << "Ошибка: индекс выходит за пределы вектора свободных членов." << endl;
    }
}

void GenerateRandom(Node& node, int size) {
    random_device rd;  
    mt19937 gen(rd()); 
    uniform_real_distribution<double> dis(-100.0, 100.0); 

    node.coefficients.reserve(size);
    node.freeTerms.reserve(size);

    bool hasUniqueSolution = false;

    while (!hasUniqueSolution) {
        node.coefficients.clear();
        node.freeTerms.clear();
        node.coefficients.resize(size, vector<double>(size));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                node.coefficients[i][j] = dis(gen);
            }
        }

        node.freeTerms.resize(size);
        for (int i = 0; i < size; ++i) {
            node.freeTerms[i] = dis(gen);
        }

        hasUniqueSolution = CheckOneSol(node);

        if (!hasUniqueSolution) {
            cout << "Система не имеет единственного решения. Генерация новой системы..." << endl;
        }
    }

    cout << "Генерация случайной системы завершена. Система имеет единственное решение." << endl;
}
