#include "InputValue.h"
#include <iostream>

using namespace std;

int InputInt(int left, int right) {
    int c;
    while (true) {

        cin >> c;
        if ((cin.good() && cin.peek() == '\n')) {
            if (c >= left && c <= right) {
                return c;               
            }
            cout << "�������� ������ ���� ������ " << left << " � ������ " << right << endl;
            cin.clear();
            cin.ignore(32767, '\n');
            continue;
            
        }
        else {
            cout << '\n' << "������������ ��������. ��������� ����." << '\n';
            cin.clear();
            cin.ignore(32767, '\n');
        }
    }
}

double InputDouble() {
    double c;
    while (true) {

        cin >> c;
        if ((cin.good() && cin.peek() == '\n')) {
            return c;
        }
        else {
            cout << '\n' << "������������ ��������. ��������� ����." << '\n';
            cin.clear();
            cin.ignore(32767, '\n');
        }
    }
}

