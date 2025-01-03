#include "DataBase.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

vector<Node> LoadDB() {
    vector<Node> db;
    ifstream file("database.txt");

    if (!file.is_open()) {
        throw runtime_error("База данных не существует!");
    }

    string line;
    while (getline(file, line)) {
        Node node;
        stringstream ss(line);
        int matrix_size;
        ss >> matrix_size;

        node.coefficients.resize(matrix_size, vector<double>(matrix_size));
        node.freeTerms.resize(matrix_size);

        for (int i = 0; i < matrix_size; i++) {
            for (int j = 0; j < matrix_size; j++) {
                ss >> node.coefficients[i][j];
            }
        }

        for (int i = 0; i < matrix_size; i++) {
            ss >> node.freeTerms[i];
        }

        ss >> node.time1;
        if (node.time1 != -1) {
            node.solution1.resize(matrix_size);
            for (int i = 0; i < matrix_size; i++) {
                ss >> node.solution1[i];
            }
        }

        ss >> node.time2;
        if (node.time2 != -1) {
            node.solution2.resize(matrix_size);
            for (int i = 0; i < matrix_size; i++) {
                ss >> node.solution2[i];
            }
        }

        ss >> node.time3;
        if (node.time3 != -1) {
            node.solution3.resize(matrix_size);
            for (int i = 0; i < matrix_size; i++) {
                ss >> node.solution3[i];
            }
        }

        ss >> node.time4;
        if (node.time4 != -1) {
            node.solution4.resize(matrix_size);
            for (int i = 0; i < matrix_size; i++) {
                ss >> node.solution4[i];
            }
        }

        db.push_back(node);
    }

    file.close();
    if (db.empty()) {
        //throw runtime_error("Нет элементов в базе данных!");
    }

    return db;
}

void SaveDB(vector<Node> list) {
    Sort(list);
    ofstream file("database.txt");

    if (!file.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }

    for (const auto& node : list) {
        file << node.GetMatrixSize() << " ";  
        for (const auto& row : node.coefficients) {
            for (const auto& elem : row) {
                file << elem << " ";
            }
        }

        for (const auto& term : node.freeTerms) {
            file << term << " ";
        }

        file << node.time1 << " ";
        if (node.time1 != -1) {
            for (const auto& sol : node.solution1) file << sol << " ";
        }

        file << node.time2 << " ";
        if (node.time2 != -1) {
            for (const auto& sol : node.solution2) file << sol << " ";
        }

        file << node.time3 << " ";
        if (node.time3 != -1) {
            for (const auto& sol : node.solution3) file << sol << " ";
        }

        file << node.time4 << " ";
        if (node.time4 != -1) {
            for (const auto& sol : node.solution4) file << sol << " ";
        }

        file << endl;
    }

    file.close();
}

void Sort(vector<Node>& List) {
    size_t n = List.size();
    if (n <= 1) return;  

    int h = n / 2;  

    while (h > 0) {
        for (size_t i = h; i < n; ++i) {
            Node tmp = List[i];
            size_t j = i;

            while (j >= h && List[j - h].GetMatrixSize() > tmp.GetMatrixSize()) {
                List[j] = List[j - h];
                j -= h;
            }
            List[j] = tmp;
        }

        h /= 2;
    }
}

Node LoadNode(int index) {
    vector<Node> db = LoadDB();
    if (index >= 0 && index < db.size()) {
        return db[index];
    }
    else {
        cerr << "Некорректный индекс!" << endl;
        return Node(); 
    }
}

void SaveNode(Node node) {
    vector<Node> db = LoadDB();
    int ind = CheckNode(db, node);
    if (ind >= 0) {
        db[ind] = node;
        SaveDB(db);
        return;
    }
    db.push_back(node);
    Sort(db);
    SaveDB(db);
}

void PrintDB(const vector<Node>& list) {
    for (size_t i = 0; i < list.size(); ++i) {
        const Node& node = list[i];
        cout << "Node #" << i << " (" << node.GetMatrixSize() << "x" << node.GetMatrixSize() << "):" << endl;
        PrintNode(node);
    }
}

int CheckNode(const vector<Node>& list, Node& node) {
    for (size_t i = 0; i < list.size(); i++) {
        if (node.GetMatrixSize() < list[i].GetMatrixSize()) {
            return -1;
        }
        if (node.GetMatrixSize() > list[i].GetMatrixSize()) {
            continue;
        }
        if (node == list[i]) {
            return i;              
        }
 
    }
    return -1;
    
}

void UpdateNode(Node node) {
    vector<Node> db = LoadDB();
    int ind = CheckNode(db, node);
    if (ind >= 0) {
        node = db[ind];
        return;
    }
}

void DeleteNodeFromDB(int index) {
    vector<Node> db = LoadDB();

    if (db.empty()) {
        throw runtime_error("Ошибка: база данных пуста!");
    }

    if (index < 0 || index >= db.size()) {
        throw runtime_error("Ошибка: индекс вне диапазона базы данных!");
    }

    db.erase(db.begin() + index); 

    SaveDB(db); 

    cout << "Узел с индексом " << index << " успешно удален из базы данных." << endl;
}

void CreateDB() {
    ofstream file("database.txt", ios::trunc); 

    if (!file.is_open()) {
        throw runtime_error("Ошибка: не удалось создать базу данных!");
    }

    cout << "Новая база данных успешно создана." << endl;

    file.close();
}

