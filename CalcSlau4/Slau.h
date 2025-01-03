#pragma once

#include "InputSlau.h"
#include <vector>

using namespace std;

void SetTime(Node& node, int index, double value);
void SetSolution(Node& node, int index, const vector<double>& solution);
void PrintNode(const Node& node);
double Determinant(const vector<vector<double>>& matrix);
void Gauss(Node& node);
vector<vector<double>> InvertMatrix(const vector<vector<double>>& A);
vector<double> MatXVec(const vector<vector<double>>& A, const vector<double>& x);
void InvertSLAU(Node& node);
void Cramer(Node& node);
vector<double> ForwardSubstitution(const vector<vector<double>>& L, const vector<double>& b);
vector<double> BackSubstitution(const vector<vector<double>>& U, const vector<double>& y);
pair<vector<vector<double>>, vector<vector<double>>> LUDecompose(const vector<vector<double>>& A);
void LUDecomposition(Node& node);