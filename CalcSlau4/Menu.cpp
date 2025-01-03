#include "Menu.h"
#include <iostream>

using namespace std;

void MainMenu(Node& node) {
    while (true) {
        cout << "\n=== ������� ���� ===" << endl;
        cout << "1. ���� ������" << endl;
        cout << "2. ������ ����" << endl;
        cout << "3. ���� ������" << endl;
        cout << "4. �����" << endl;
        cout << "�������� ����� ����: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "�������� ����. ����������, ������� ����� �� 1 �� 4." << endl;
            continue; 
        }

        switch (choice) {
        case 1:
            InputMenu(node);
            continue;

        case 2:
            SolveMenu(node);
            continue;

        case 3:
            DataBaseMenu();
            continue;

        case 4:
            cout << "���������� ���������." << endl;
            return;

        default:
            cout << "�������� �����. ���������� �����." << endl;
            break;
        }
    }
}

void InputMenu(Node& node) {
    int choice;
    bool OneSol = true;
    while (true) {
        cout << "\n=== ���� ������ ===" << endl;
        cout << "1. ���� ������� �������� ���������" << endl;
        cout << "2. ������� ���� �� ���� ������" << endl;
        cout << "3. ������������� ��������� ����" << endl;
        cout << "4. ��������� ��������� �������" << endl;
        cout << "5. ��������� ��������� ������� ��������� ������" << endl;
        cout << "6. �������� ����" << endl;
        cout << "7. ����� � ������� ����" << endl;
        cout << "�������� �����: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "�������� ����." << endl;
            continue; 
        }
        switch (choice) {
        case 1: {
            cout << "\n������� ������ ������� (����������� ������ = 3): ";
            int size = InputInt(3);
            node.coefficients.resize(size);
            for (auto& row : node.coefficients) {
                row.resize(size);
            }
            node.freeTerms.resize(size);
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cout << "������� �������� �������� [" << i + 1 << "][" << j + 1 << "]: ";
                    double value = InputDouble();
                    ModifyCoef(node, i, j, value);
                }
                cout << "������� ������� ������� ��������� ������ [" << i + 1 << "]: ";
                double value = InputDouble();
                ModifyFreeTerms(node, i, value);
            }
            PrintNode(node);
            OneSol = CheckOneSol(node);
            if (OneSol) {
                cout << "\n������ ���� ����� ���� �������." << "\n����������� ����� � ������� ����." << endl;
                return;
            }
            cout << "\n������ ���� �� ����� ���� �������." << endl;
            continue;
        }

        case 2: {
            try {
                vector<Node> db = LoadDB();
                if (db.empty()) {
                    cout << "\n���� ������ �����";
                    continue;
                }
                PrintDB(db);
                cout << "\n������� ������ ���������� ����: ";
                int index = InputInt(0, db.size());
                node = LoadNode(index);
            }
            catch (const runtime_error& e) {
                cerr << "\n������: " << e.what() << endl;
            }
            return;
        }

        case 3: {
            cout << "������� ������ ������� (����������� ������ = 3) : ";
            int size = InputInt(3);
            GenerateRandom(node, size);
            PrintNode(node);
            return;
        }

        case 4: {
            if (IsEmpty(node)) {
                cout << "\n���� ����.\n������� ���� � ������� ���� '���� ����'." << endl;
                continue;
            }
            while (true) {
                cout << "������� 'exit' ��� ����������: ";
                string input;
                cin >> input;

                if (input == "exit") break;

                cout << "������� ������ ������ ������� (���������� � 1): ";
                int i = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "������� ������ ������� ������� (���������� � 1): ";
                int j = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "������� ����� �������� ��������: ";
                double value = InputDouble();
                ModifyCoef(node, i, j, value);
            }
            OneSol = CheckOneSol(node);
            PrintNode(node);
            if (OneSol) {
                cout << "\n������ ���� ����� ���� �������." << "\n����������� ����� � ������� ����." << endl;
                return;
            }
            cout << "\n������ ���� �� ����� ���� �������." << endl;
            continue;
            
        }

        case 5: {
            if (IsEmpty(node)) {
                cout << "\n���� ����.\n������� ���� � ������� ���� '���� ����'." << endl;
                continue;
            }
            while (true) {
                cout << "������� 'exit' ��� ����������: ";
                string input;
                cin >> input;

                if (input == "exit") break;

                cout << "������� ������ ������ ������� ��������� ������ (���������� � 1): ";
                int i = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "������� ����� �������� ��������: ";
                double value = InputDouble();
                ModifyFreeTerms(node, i, value);
            }
            PrintNode(node);
            OneSol = CheckOneSol(node);
            if (OneSol) {
                cout << "\n������ ���� ����� ���� �������." << "\n����������� ����� � ������� ����." << endl;
                return;
            }
            cout << "\n������ ���� �� ����� ���� �������." << endl;
            continue;
        }
        case 6:
            ClearNode(node);
            cout << "\n ���� ������� �������";
            continue;
        case 7:
            if (!OneSol) {
                cout << "\n��� ������ �� ������� ���� ����, � ������� ��� ������������� �������, ���������" << endl;
                cout << "���� ������ �����, ������� 'exit'" << endl;
                string input;
                cin >> input;
                if(input == "exit"){
                    ClearNode(node);
                    return;
                }
                cout << "\n����������� � ���� ����� ����" << endl;
                continue;
            }
            else {
                return;
            }
        default:
            cout << "�������� �����. ���������� �����." << endl;

        }
    }
}
void SolveMenu(Node& node) {
    if (IsEmpty(node)) {
        cout << "\n���� ���� ��� �������.\n������� ���� � ������� ���� '���� ������'." << endl;
        return;
    }
    try {
        UpdateNode(node);
    }catch (const runtime_error& e) {
        CreateDB();
    }
    while (true) {
        cout << "\n=== ����� ������ ������� ===" << endl;
        cout << "1. ����� ������" << endl;
        cout << "2. ����� �������� �������" << endl;
        cout << "3. ����� �������" << endl;
        cout << "4. ����� LU-����������" << endl;
        cout << "5. ��������� � ������� ����" << endl;
        cout << "�������� �����: ";

        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "�������� ����." << endl;
            continue; 
        }

        switch (choice) {
        case 1:
            Gauss(node);
            PrintNode(node);
            SaveNode(node);
            continue;
        case 2:
            InvertSLAU(node);
            PrintNode(node);
            SaveNode(node);
            continue;
        case 3:
            Cramer(node);
            PrintNode(node);
            SaveNode(node);
            continue;
        case 4:
            LUDecomposition(node);
            PrintNode(node);
            SaveNode(node);
            continue;
        case 5:
            return;
        default:
            cout << "�������� �����. ���������� �����." << endl;
        }
    }
}

void DataBaseMenu() {
    while (true) {
        cout << "\n=== ���� ������ ===" << endl;
        cout << "1. ��������/�������� ��" << endl;
        cout << "2. �������� ��������" << endl;
        cout << "3. ������� ��" << endl;
        cout << "4. �����" << endl;
        cout << "�������� ����� ����: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "�������� ����." << endl;
            continue; 
        }

        switch (choice) {
        case 1:
            try {
                CreateDB();
            }
            catch (const runtime_error& e) {
                cerr << "\n������: " << e.what() << endl;
            }
            continue;

        case 2: {
            cout << "������� ������ ���� ��� ��������: ";
            int index = InputInt(0);
            try {
                DeleteNodeFromDB(index);
            }
            catch (const runtime_error& e) {
                cerr << "\n������: " << e.what() << endl;
            }
            continue;
        }
        case 3:
            try {
                vector<Node> db = LoadDB();
                if (db.empty()) {
                    cout << "\n���� ������ �����." << endl;
                }
                PrintDB(db);
            }
            catch (const runtime_error& e) {
                cerr << "\n������: " << e.what() << endl;
            }
            continue;

        case 4:
            cout << "\n����������� � ������� ����." << endl;
            return;

        default:
            cout << "�������� �����. ���������� �����." << endl;
        }
    }
}