#include "Slau.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <stdexcept>

using namespace std;

void SetTime(Node& node, int index, double value) {
    switch (index) {
    case 1:
        node.time1 = value;
        break;
    case 2:
        node.time2 = value;
        break;
    case 3:
        node.time3 = value;
        break;
    case 4:
        node.time4 = value;
        break;
    default:
        throw out_of_range("Invalid index for time");
    }
}

void SetSolution(Node& node, int index, const vector<double>& solution) {
    switch (index) {
    case 1:
        node.solution1 = solution;
        break;
    case 2:
        node.solution2 = solution;
        break;
    case 3:
        node.solution3 = solution;
        break;
    case 4:
        node.solution4 = solution;
        break;
    default:
        throw out_of_range("Invalid index for solution");
    }
}

void PrintNode(const Node& node) {
    cout << "Коэффициенты системы:" << endl;
    for (const auto& row : node.coefficients) {
        for (double coeff : row) {
            cout << setw(10) << coeff << " ";
        }
        cout << endl;
    }

    cout << "\nСвободные члены:" << endl;
    for (double freeTerm : node.freeTerms) {
        cout << setw(10) << freeTerm << " ";
    }
    cout << endl;

    cout << "\nТаблица с решениями и временем их выполнения:" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "Метод                  | Время (сек)  | Решения" << endl;
    cout << "------------------------------------------------------------" << endl;


    if (node.time1 > 0) {
        cout << "Метод Гаусса           | " << setw(12) << node.time1 << " | ";
        for (double sol : node.solution1) {
            cout << sol << " ";
        }
        cout << endl;
    }


    if (node.time2 > 0) {
        cout << "Метод обратной матрицы | " << setw(12) << node.time2 << " | ";
        for (double sol : node.solution2) {
            cout << sol << " ";
        }
        cout << endl;
    }


    if (node.time3 > 0) {
        cout << "Метод Крамера          | " << setw(12) << node.time3 << " | ";
        for (double sol : node.solution3) {
            cout << sol << " ";
        }
        cout << endl;
    }


    if (node.time4 > 0) {
        cout << "Метод LU-разложения    | " << setw(12) << node.time4 << " | ";
        for (double sol : node.solution4) {
            cout << sol << " ";
        }
        cout << endl;
    }

    cout << "------------------------------------------------------------" << endl;
}


double Determinant(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    double det = 0.0;

    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    for (int p = 0; p < n; ++p) {
        vector<vector<double>> subMatrix(n - 1, vector<double>(n - 1));
        for (int i = 1; i < n; ++i) {
            int j = 0;
            for (int k = 0; k < n; ++k) {
                if (k != p) {
                    subMatrix[i - 1][j] = matrix[i][k];
                    j++;
                }
            }
        }
        det += (p % 2 == 0 ? 1 : -1) * matrix[0][p] * Determinant(subMatrix);
    }

    return det;
}

void Gauss(Node& node) {
    if (node.time1 >= 0) {
        return;
    }
    auto start = chrono::high_resolution_clock::now();

    int n = node.coefficients.size();
    vector<vector<double>> augmented(n, vector<double>(n + 1));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented[i][j] = node.coefficients[i][j];
        }
        augmented[i][n] = node.freeTerms[i];
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (augmented[i][i] == 0) throw runtime_error("Система не имеет единственного решения");
            double ratio = augmented[j][i] / augmented[i][i];
            for (int k = i; k <= n; ++k) {
                augmented[j][k] -= ratio * augmented[i][k];
            }
        }
    }

    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = augmented[i][n];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= augmented[i][j] * x[j];
        }
        x[i] /= augmented[i][i];
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    SetTime(node, 1, duration.count());
    SetSolution(node, 1, x);
}

vector<vector<double>> InvertMatrix(const vector<vector<double>>& A) {
    int n = A.size();
    vector<vector<double>> augmented(n, vector<double>(2 * n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented[i][j] = A[i][j];
        }
        augmented[i][n + i] = 1.0;
    }

    for (int i = 0; i < n; i++) {
        double pivot = augmented[i][i];
        if (pivot == 0) {
            throw runtime_error("Матрица вырожденная, обратной не существует");
        }

        for (int j = 0; j < 2 * n; j++) {
            augmented[i][j] /= pivot;
        }

        for (int k = 0; k < n; k++) {
            if (k != i) {
                double factor = augmented[k][i];
                for (int j = 0; j < 2 * n; j++) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    vector<vector<double>> inverse(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse[i][j] = augmented[i][n + j];
        }
    }

    return inverse;
}

vector<double> MatXVec(const vector<vector<double>>& A, const vector<double>& x) {
    int n = A.size();
    vector<double> result(n, 0.0);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * x[j];
        }
    }

    return result;
}

void InvertSLAU(Node& node) {
    if (node.time2 >= 0) {
        return;
    }
    auto start = chrono::high_resolution_clock::now();

    vector<vector<double>> inverse = InvertMatrix(node.coefficients);
    vector<double> x = MatXVec(inverse, node.freeTerms);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    SetTime(node, 2, duration.count());  
    SetSolution(node, 2, x);  
}



void Cramer(Node& node) {
    if (node.time3 >= 0) {
        return;
    }
    auto start = chrono::high_resolution_clock::now();

    int n = node.coefficients.size();
    double detA = Determinant(node.coefficients);
    if (detA == 0) {
        throw runtime_error("Определитель равен нулю. Система не имеет единственного решения.");
    }

    vector<double> solutions(n);

    for (int i = 0; i < n; ++i) {
        vector<vector<double>> tempMatrix = node.coefficients;
        for (int j = 0; j < n; ++j) {
            tempMatrix[j][i] = node.freeTerms[j];
        }
        solutions[i] = Determinant(tempMatrix) / detA;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    SetTime(node, 3, duration.count());
    SetSolution(node, 3, solutions);
}

pair<vector<vector<double>>, vector<vector<double>>> LUDecompose(const vector<vector<double>>& A) {
    int n = A.size();
    vector<vector<double>> L(n, vector<double>(n, 0));
    vector<vector<double>> U(n, vector<double>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            U[i][j] = A[i][j];
            for (int k = 0; k < i; ++k) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        for (int j = i; j < n; ++j) {
            if (i == j) {
                L[i][i] = 1;
            }
            else {
                L[j][i] = A[j][i];
                for (int k = 0; k < i; ++k) {
                    L[j][i] -= L[j][k] * U[k][i];
                }
                L[j][i] /= U[i][i];
            }
        }
    }

    return make_pair(L, U);
}

void LUDecomposition(Node& node) {
    if (node.time4 >= 0) {
        return;
    }
    auto start = chrono::high_resolution_clock::now();

    int n = node.coefficients.size();
    vector<vector<double>> A = node.coefficients;
    vector<double> b = node.freeTerms;

    pair<vector<vector<double>>, vector<vector<double>>> LU = LUDecompose(A);
    vector<vector<double>> L = LU.first;
    vector<vector<double>> U = LU.second;

    auto y = ForwardSubstitution(L, b);
    vector<double> x = BackSubstitution(U, y);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    SetTime(node, 4, duration.count());
    SetSolution(node, 4, x);
}

vector<double> ForwardSubstitution(const vector<vector<double>>& L, const vector<double>& b) {
    int n = L.size();
    vector<double> y(n);

    for (int i = 0; i < n; ++i) {
        double sum = 0.0;
        for (int j = 0; j < i; ++j) {
            sum += L[i][j] * y[j];
        }
        y[i] = (b[i] - sum) / L[i][i];
    }

    return y;
}


vector<double> BackSubstitution(const vector<vector<double>>& U, const vector<double>& y) {
    int n = U.size();
    vector<double> x(n);

    for (int i = n - 1; i >= 0; --i) {
        double sum = 0.0;
        for (int j = i + 1; j < n; ++j) {
            sum += U[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / U[i][i];
    }

    return x;
}
