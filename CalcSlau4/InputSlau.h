#pragma once

#include <vector>

using namespace std;

struct Node {
    vector<vector<double>> coefficients;
    vector<double> freeTerms;
    double time1 = -1;
    vector<double> solution1;
    double time2 = -1;
    vector<double> solution2;
    double time3 = -1;
    vector<double> solution3;
    double time4 = -1;
    vector<double> solution4;

    bool operator==(const Node& other) const;
    int GetMatrixSize() const;
};

bool CompareDoubles(double a, double b);

bool IsEmpty(const Node& node);

void ClearNode(Node& node);

int Rank(vector<vector<double>> matrix);

bool CheckOneSol(const Node& node);

void ModifyCoef(Node& node, int i, int j, double value);

void ModifyFreeTerms(Node& node, int i, double value);

void GenerateRandom(Node& node, int size);