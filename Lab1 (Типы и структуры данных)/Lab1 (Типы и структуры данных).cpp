#include <iostream>
#include <vector>
#include <algorithm>    // Для std::sort, std::equal_range и std::shuffle
#include <cstdlib>      // Для system()
#include <limits>       // Для numeric_limits
#include <sstream>      // Для istringstream (парсинг строки)
#include <chrono>       // Для измерения времени
#include <random>       // Для случайного заполнения
#include <ctime>
using namespace std;
using namespace std::chrono;

// Шаблон функции для проверки ввода
template <typename T>
T getValidatedInput(const string& prompt) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        else {
            cout << "Ошибка: неверный ввод. Попробуйте снова.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Прототипы функций
void clear_console();
void input_vector(vector<double>& arr);
void output_vector(const vector<double>& arr);
vector<int> linear_search_all(const vector<double>& arr, double target);
vector<int> binary_search_all(const vector<double>& arr, double target);
void random_fill_positions(vector<double>& arr);
void random_fill_all(vector<double>& arr);  // Функция для полного случайного заполнения

int main() {
    setlocale(LC_ALL, "Rus");

    int arr_size = 0;
    vector<double> arr;              // Используем double для большей точности
    bool vectorInitialized = false;  // Флаг, показывающий, введён ли массив
    int choice = 0;                  // Выбор пользователя в меню

    // Основной цикл меню
    do {
        clear_console();  // Очищаем консоль перед выводом меню

        cout << "\n=== Главное меню ===\n";
        cout << "1. Ввести новый массив\n";
        cout << "2. Вывести текущий массив\n";
        cout << "3. Линейный поиск (неотсортированный массив)\n";
        cout << "4. Сортировка массива\n";
        cout << "5. Бинарный поиск (отсортированный массив)\n";
        cout << "6. Случайное заполнение массива выбранных позиций\n";
        cout << "7. Выход\n";
        choice = getValidatedInput<int>("Введите ваш выбор (или 0 для отмены операции): ");
        if (choice == 0) {
            cout << "Операция отменена.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            // Ввод нового массива
            bool validInput = false;
            bool operationCancelled = false;
            while (!validInput) {
                arr_size = getValidatedInput<int>("\nВведите размер массива (или 0 для отмены): ");
                if (arr_size == 0) {
                    cout << "Операция отменена.\n";
                    operationCancelled = true;
                    break;
                }
                if (arr_size < 0) {
                    cout << "Размер массива должен быть положительным числом.\n";
                    continue;
                }
                if (arr_size > 10) {
                    char confirm = getValidatedInput<char>(
                        "\nВы задали большой размер массива (" + to_string(arr_size) +
                        "). Точно хотите его ввести? (Y/N, 0 для отмены): ");
                    if (confirm == '0') {
                        cout << "Операция отменена.\n";
                        operationCancelled = true;
                        break;
                    }
                    if (confirm == 'Y' || confirm == 'y') {
                        validInput = true;
                    }
                    else {
                        cout << "Попробуйте снова.\n";
                    }
                }
                else {
                    validInput = true;
                }
            }
            if (operationCancelled)
                break;
            arr.resize(arr_size);

            // Выбор способа заполнения массива
            char randomChoice = getValidatedInput<char>(
                "\nЗаполнить массив случайными числами? (Y/N, 0 для отмены): ");
            if (randomChoice == '0') {
                cout << "Операция отменена.\n";
                break;
            }
            if (randomChoice == 'Y' || randomChoice == 'y') {
                random_fill_all(arr);
            }
            else {
                input_vector(arr);
            }
            vectorInitialized = true;
            break;
        }
        case 2: {
            char bigChoice = 'Y';
            if (arr_size > 10000) {
                bigChoice = getValidatedInput<char>(
                    "\nВывод массива может быть долгим. Вы точно хотите его вывести? (Y/N, 0 для отмены): ");
                if (bigChoice == '0') {
                    cout << "Операция отменена.\n";
                    break;
                }
            }
            if (bigChoice == 'N' || bigChoice == 'n') {
                break;
            }
            if (!vectorInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
            }
            else {
                cout << "\nТекущий массив:\n";
                output_vector(arr);
            }
            system("pause");
            break;
        }
        case 3: {
            // Линейный поиск в неотсортированном массиве
            if (!vectorInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            double search_value = getValidatedInput<double>(
                "\nВведите значение для линейного поиска (или 0 для отмены): ");
            if (search_value == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            auto start = high_resolution_clock::now();
            vector<int> indices = linear_search_all(arr, search_value);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            double duration_ms = duration / 1000.0;

            if (!indices.empty()) {
                // Если массив очень большой, спрашиваем, выводить ли все найденные позиции
                if (arr.size() > 1000000) {
                    char printChoice = getValidatedInput<char>(
                        "\nРазмер массива больше 1 000 000. Вы действительно хотите вывести найденные позиции? (Y/N, 0 для отмены): ");
                    if (printChoice == '0') {
                        cout << "Операция отменена.\n";
                        break;
                    }
                    if (printChoice == 'Y' || printChoice == 'y') {
                        cout << "Элемент " << search_value << " найден на позициях (индексация с 0): ";
                        for (int idx : indices) {
                            cout << idx << " ";
                        }
                        cout << endl;
                    }
                    else {
                        cout << "Найдено " << indices.size() << " совпадений.\n";
                    }
                }
                else {
                    cout << "Элемент " << search_value << " найден на позициях (индексация с 0): ";
                    for (int idx : indices) {
                        cout << idx << " ";
                    }
                    cout << endl;
                }
            }
            else {
                cout << "Элемент " << search_value << " не найден в массиве." << endl;
            }
            cout << "\nВремя выполнения линейного поиска: " << duration << " мкс ("
                << duration_ms << " мс)" << endl;
            system("pause");
            break;
        }
        case 4: {
            // Сортировка массива
            if (!vectorInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
            }
            else {
                sort(arr.begin(), arr.end());
                cout << "\nМассив отсортирован.\n";
            }
            system("pause");
            break;
        }
        case 5: {
            // Бинарный поиск в отсортированном массиве
            if (!vectorInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            double search_value = getValidatedInput<double>(
                "\nВведите значение для бинарного поиска (или 0 для отмены): ");
            if (search_value == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            auto start = high_resolution_clock::now();
            vector<int> indices = binary_search_all(arr, search_value);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            double duration_ms = duration / 1000.0;

            if (!indices.empty()) {
                if (arr.size() > 1000000) {
                    char printChoice = getValidatedInput<char>(
                        "\nРазмер массива больше 1 000 000. Вы действительно хотите вывести найденные позиции? (Y/N, 0 для отмены): ");
                    if (printChoice == '0') {
                        cout << "Операция отменена.\n";
                        break;
                    }
                    if (printChoice == 'Y' || printChoice == 'y') {
                        cout << "Элемент " << search_value << " найден на позициях (индексация с 0): ";
                        for (int idx : indices) {
                            cout << idx << " ";
                        }
                        cout << endl;
                    }
                    else {
                        cout << "Найдено " << indices.size() << " совпадений.\n";
                    }
                }
                else {
                    cout << "Элемент " << search_value << " найден на позициях (индексация с 0): ";
                    for (int idx : indices) {
                        cout << idx << " ";
                    }
                    cout << endl;
                }
            }
            else {
                cout << "Элемент " << search_value << " не найден в массиве." << endl;
            }
            cout << "\nВремя выполнения бинарного поиска: " << duration << " мкс ("
                << duration_ms << " мс)" << endl;
            system("pause");
            break;
        }
        case 6: {
            // Случайное заполнение массива выбранных позиций
            if (!vectorInitialized) {
                cout << "\nМассив не введён. Сначала создайте массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            int n = getValidatedInput<int>("\nВведите количество позиций для случайного заполнения (или 0 для отмены): ");
            if (n == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            if (n > arr.size()) {
                n = arr.size();
                cout << "Введенное количество превышает размер массива. Будут заполнены все позиции.\n";
            }
            // Создаем вектор индексов от 0 до arr.size()-1
            vector<int> indices(arr.size());
            for (int i = 0; i < arr.size(); i++) {
                indices[i] = i;
            }
            // Перемешиваем индексы
            random_device rd;
            mt19937 g(rd());
            shuffle(indices.begin(), indices.end(), g);
            // Генерируем числа с одним знаком после запятой:
            // Генерируем целое число от 0 до 1000 и делим на 10.0.
            uniform_int_distribution<int> dist(0, 1000);
            for (int i = 0; i < n; i++) {
                int idx = indices[i];
                arr[idx] = dist(g) / 10.0;
            }
            cout << "Случайное заполнение выполнено для " << n << " позиций." << endl;
            system("pause");
            break;
        }
        case 7: {
            cout << "\nВыход из программы. До свидания!\n";
            break;
        }
        default:
            cout << "\nНекорректный выбор. Попробуйте снова.\n";
            system("pause");
            break;
        }
    } while (choice != 7);

    return 0;
}

//------------------------------------------------------------------------------
// Функция для очистки консоли
// Использует команду "cls" для Windows и "clear" для других ОС
//------------------------------------------------------------------------------
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//------------------------------------------------------------------------------
// Функция ввода элементов вектора (горизонтальный ввод)
//------------------------------------------------------------------------------
void input_vector(vector<double>& arr) {
    bool valid = false;
    while (!valid) {
        cout << "\n=== Ввод элементов массива ===\n";
        cout << "Введите " << arr.size() << " чисел, разделенных пробелами (или введите 0 для отмены): ";

        string line;
        getline(cin, line);
        if (line.empty()) { // Если предыдущий ввод оставил пустую строку
            getline(cin, line);
        }
        if (line == "0") {
            cout << "Операция отменена.\n";
            return;
        }

        istringstream iss(line);
        vector<double> temp;
        double value;
        while (iss >> value) {
            temp.push_back(value);
        }

        if (temp.size() != arr.size()) {
            cout << "Ошибка: введено " << temp.size() << " чисел, ожидалось " << arr.size() << ". Попробуйте снова.\n";
        }
        else {
            arr = temp;
            valid = true;
        }
    }
    cout << "\n--- Ввод завершен! ---\n";
    system("pause");
}

//------------------------------------------------------------------------------
// Функция вывода элементов вектора
//------------------------------------------------------------------------------
void output_vector(const vector<double>& arr) {
    for (double num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

//------------------------------------------------------------------------------
// Функция линейного поиска всех вхождений элемента в неотсортированном векторе
//------------------------------------------------------------------------------
vector<int> linear_search_all(const vector<double>& arr, double target) {
    vector<int> indices;
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == target) {
            indices.push_back(i);
        }
    }
    return indices;
}

//------------------------------------------------------------------------------
// Функция бинарного поиска всех вхождений элемента в отсортированном векторе
//------------------------------------------------------------------------------
vector<int> binary_search_all(const vector<double>& arr, double target) {
    vector<int> indices;
    auto range = equal_range(arr.begin(), arr.end(), target);
    for (auto it = range.first; it != range.second; ++it) {
        indices.push_back(distance(arr.begin(), it));
    }
    return indices;
}

//------------------------------------------------------------------------------
// Функция случайного заполнения массива выбранных позиций случайными значениями
//------------------------------------------------------------------------------
void random_fill_positions(vector<double>& arr) {
    if (arr.empty()) {
        cout << "Массив не создан. Сначала создайте массив (пункт 1 меню).\n";
        return;
    }
    int n = getValidatedInput<int>("\nВведите количество позиций для случайного заполнения (или 0 для отмены): ");
    if (n == 0) {
        cout << "Операция отменена.\n";
        return;
    }
    if (n > arr.size()) {
        n = arr.size();
        cout << "Введенное количество превышает размер массива. Будут заполнены все позиции.\n";
    }
    // Создаем вектор индексов от 0 до arr.size()-1
    vector<int> indices(arr.size());
    for (int i = 0; i < arr.size(); i++) {
        indices[i] = i;
    }
    // Перемешиваем индексы
    random_device rd;
    mt19937 g(rd());
    shuffle(indices.begin(), indices.end(), g);
    // Генерируем числа с одним знаком после запятой:
    // Генерируем целое число от 0 до 1000 и делим на 10.0.
    uniform_int_distribution<int> dist(0, 1000);
    for (int i = 0; i < n; i++) {
        int idx = indices[i];
        arr[idx] = dist(g) / 10.0;
    }
    cout << "Случайное заполнение выполнено для " << n << " позиций." << endl;
}

//------------------------------------------------------------------------------
// Функция случайного заполнения всего массива случайными значениями
//------------------------------------------------------------------------------
void random_fill_all(vector<double>& arr) {
    if (arr.empty()) {
        cout << "Массив не создан. Сначала создайте массив (пункт 1 меню).\n";
        return;
    }
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<int> dist(0, 1000);
    for (size_t i = 0; i < arr.size(); i++) {
        arr[i] = dist(g) / 10.0;
    }
    cout << "Массив заполнен случайными числами с одним знаком после запятой." << endl;
}
