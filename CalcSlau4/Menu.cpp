#include "Menu.h"
#include <iostream>

using namespace std;

void MainMenu(Node& node) {
    while (true) {
        cout << "\n=== Главное меню ===" << endl;
        cout << "1. Ввод данных" << endl;
        cout << "2. Решить СЛАУ" << endl;
        cout << "3. База данных" << endl;
        cout << "4. Выход" << endl;
        cout << "Выберите пункт меню: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Неверный ввод. Пожалуйста, введите число от 1 до 4." << endl;
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
            cout << "Завершение программы." << endl;
            return;

        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    }
}

void InputMenu(Node& node) {
    int choice;
    bool OneSol = true;
    while (true) {
        cout << "\n=== Ввод данных ===" << endl;
        cout << "1. Ввод системы линейных уравнений" << endl;
        cout << "2. Выбрать СЛАУ из базы данных" << endl;
        cout << "3. Сгенерировать случайное СЛАУ" << endl;
        cout << "4. Изменение элементов матрицы" << endl;
        cout << "5. Изменение элементов вектора свободных членов" << endl;
        cout << "6. Очистить СЛАУ" << endl;
        cout << "7. Выход в главное меню" << endl;
        cout << "Выберите пункт: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Неверный ввод." << endl;
            continue; 
        }
        switch (choice) {
        case 1: {
            cout << "\nВведите размер матрицы (минимальный размер = 3): ";
            int size = InputInt(3);
            node.coefficients.resize(size);
            for (auto& row : node.coefficients) {
                row.resize(size);
            }
            node.freeTerms.resize(size);
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    cout << "Введите значение элемента [" << i + 1 << "][" << j + 1 << "]: ";
                    double value = InputDouble();
                    ModifyCoef(node, i, j, value);
                }
                cout << "Введите элемент вектора свободных членов [" << i + 1 << "]: ";
                double value = InputDouble();
                ModifyFreeTerms(node, i, value);
            }
            PrintNode(node);
            OneSol = CheckOneSol(node);
            if (OneSol) {
                cout << "\nДанное СЛАУ имеет одно решение." << "\nВыполняется выход в главное меню." << endl;
                return;
            }
            cout << "\nДанное СЛАУ не имеет одно решение." << endl;
            continue;
        }

        case 2: {
            try {
                vector<Node> db = LoadDB();
                if (db.empty()) {
                    cout << "\nБаза данных пуста";
                    continue;
                }
                PrintDB(db);
                cout << "\nВведите индекс выбранного СЛАУ: ";
                int index = InputInt(0, db.size());
                node = LoadNode(index);
            }
            catch (const runtime_error& e) {
                cerr << "\nОшибка: " << e.what() << endl;
            }
            return;
        }

        case 3: {
            cout << "Введите размер матрицы (минимальный размер = 3) : ";
            int size = InputInt(3);
            GenerateRandom(node, size);
            PrintNode(node);
            return;
        }

        case 4: {
            if (IsEmpty(node)) {
                cout << "\nНету СЛАУ.\nВведите СЛАУ с помощью меню 'Ввод СЛАУ'." << endl;
                continue;
            }
            while (true) {
                cout << "Введите 'exit' для завершения: ";
                string input;
                cin >> input;

                if (input == "exit") break;

                cout << "Введите индекс строки матрицы (начинается с 1): ";
                int i = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "Введите индекс столбца матрицы (начинается с 1): ";
                int j = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "Введите новое значение элемента: ";
                double value = InputDouble();
                ModifyCoef(node, i, j, value);
            }
            OneSol = CheckOneSol(node);
            PrintNode(node);
            if (OneSol) {
                cout << "\nДанное СЛАУ имеет одно решение." << "\nВыполняется выход в главное меню." << endl;
                return;
            }
            cout << "\nДанное СЛАУ не имеет одно решение." << endl;
            continue;
            
        }

        case 5: {
            if (IsEmpty(node)) {
                cout << "\nНету СЛАУ.\nВведите СЛАУ с помощью меню 'Ввод СЛАУ'." << endl;
                continue;
            }
            while (true) {
                cout << "Введите 'exit' для завершения: ";
                string input;
                cin >> input;

                if (input == "exit") break;

                cout << "Введите индекс строки вектора свободных членов (начинается с 1): ";
                int i = InputInt(1, node.GetMatrixSize()) - 1;
                cout << "Введите новое значение элемента: ";
                double value = InputDouble();
                ModifyFreeTerms(node, i, value);
            }
            PrintNode(node);
            OneSol = CheckOneSol(node);
            if (OneSol) {
                cout << "\nДанное СЛАУ имеет одно решение." << "\nВыполняется выход в главное меню." << endl;
                return;
            }
            cout << "\nДанное СЛАУ не имеет одно решение." << endl;
            continue;
        }
        case 6:
            ClearNode(node);
            cout << "\n Слау успешно удалено";
            continue;
        case 7:
            if (!OneSol) {
                cout << "\nПри выходе из данного меню СЛАУ, у которой нет единственного решения, стирается" << endl;
                cout << "Если хотите выйти, введите 'exit'" << endl;
                string input;
                cin >> input;
                if(input == "exit"){
                    ClearNode(node);
                    return;
                }
                cout << "\nВозвращение в меню ввода СЛАУ" << endl;
                continue;
            }
            else {
                return;
            }
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;

        }
    }
}
void SolveMenu(Node& node) {
    if (IsEmpty(node)) {
        cout << "\nНету СЛАУ для решения.\nВведите СЛАУ с помощью меню 'Ввод данных'." << endl;
        return;
    }
    try {
        UpdateNode(node);
    }catch (const runtime_error& e) {
        CreateDB();
    }
    while (true) {
        cout << "\n=== Выбор метода решения ===" << endl;
        cout << "1. Метод Гаусса" << endl;
        cout << "2. Метод обратной матрицы" << endl;
        cout << "3. Метод Крамера" << endl;
        cout << "4. Метод LU-разложения" << endl;
        cout << "5. Вернуться в главное меню" << endl;
        cout << "Выберите метод: ";

        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Неверный ввод." << endl;
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
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
}

void DataBaseMenu() {
    while (true) {
        cout << "\n=== База Данных ===" << endl;
        cout << "1. Создание/очищение БД" << endl;
        cout << "2. Удаление элемента" << endl;
        cout << "3. Вывести БД" << endl;
        cout << "4. Выход" << endl;
        cout << "Выберите пункт меню: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод." << endl;
            continue; 
        }

        switch (choice) {
        case 1:
            try {
                CreateDB();
            }
            catch (const runtime_error& e) {
                cerr << "\nОшибка: " << e.what() << endl;
            }
            continue;

        case 2: {
            cout << "Введите индекс узла для удаления: ";
            int index = InputInt(0);
            try {
                DeleteNodeFromDB(index);
            }
            catch (const runtime_error& e) {
                cerr << "\nОшибка: " << e.what() << endl;
            }
            continue;
        }
        case 3:
            try {
                vector<Node> db = LoadDB();
                if (db.empty()) {
                    cout << "\nБаза данных пуста." << endl;
                }
                PrintDB(db);
            }
            catch (const runtime_error& e) {
                cerr << "\nОшибка: " << e.what() << endl;
            }
            continue;

        case 4:
            cout << "\nВозвращение в главное меню." << endl;
            return;

        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
        }
    }
}