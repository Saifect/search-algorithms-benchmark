#include <iostream>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <chrono>
#include <string>
#include <cmath>    // Для log2

using namespace std;
using namespace std::chrono;

// Шаблон функции для проверки ввода с консоли.
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

// Функция для запроса подтверждения (Y/N)
bool getConfirmation(const string& prompt) {
    char answer;
    cout << prompt;
    cin >> answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (answer == 'Y' || answer == 'y');
}

// Функция очистки экрана.
void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Функция ввода массива вручную.
void input_array(float* arr, int n) {
    cout << "\n=== Ввод элементов массива ===\n";
    for (int i = 0; i < n; i++) {
        cout << "Элемент [" << i << "]: ";
        arr[i] = getValidatedInput<float>("");
    }
}

// Функция вывода массива на экран.
void output_array(float* arr, int n) {
    cout << "\nЭлементы массива:\n";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << "\n";
}

// Реализация линейного поиска.
// Если printPositions == true, выводятся найденные индексы,
// иначе считается и выводится только количество совпадений.
void linear_search(float* arr, int n, float target, bool printPositions) {
    if (printPositions) {
        bool found = false;
        cout << "Найденные позиции: ";
        for (int i = 0; i < n; i++) {
            if (arr[i] == target) {
                cout << i << " ";
                found = true;
            }
        }
        if (!found) {
            cout << "Элемент не найден.";
        }
        cout << "\n";
    }
    else {
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (arr[i] == target)
                count++;
        }
        if (count > 0)
            cout << "Элемент " << target << " найден " << count << " раз(а).\n";
        else
            cout << "Элемент " << target << " не найден в массиве.\n";
    }
}

// Стандартная реализация бинарного поиска (итеративная версия).
int binary_search(float* arr, int n, float target) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target)
            return mid;
        else if (arr[mid] < target)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

// Функция для подсчёта количества вхождений target в отсортированном массиве,
// используя бинарный поиск для поиска одного вхождения, а затем расширяя область.
int binary_search_count(float* arr, int n, float target) {
    int index = binary_search(arr, n, target);
    if (index == -1)
        return 0;
    int count = 1;
    int left = index - 1;
    while (left >= 0 && arr[left] == target) {
        count++;
        left--;
    }
    int right = index + 1;
    while (right < n && arr[right] == target) {
        count++;
        right++;
    }
    return count;
}

// Функция сортировки методом пузырьковой сортировки.
void bubble_sort(float* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// Функция для обмена двух значений (используется в сортировках).
void swap(float& a, float& b) {
    float temp = a;
    a = b;
    b = temp;
}

/* ========= Интроспективная сортировка (Introsort) ========= */

// Сортировка вставками для небольших отрезков.
void insertion_sort(float* arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        float key = arr[i];
        int j = i - 1;
        while (j >= low && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Функция "heapify" для сортировки кучей (heapsort) в подмассиве [low; high].
void heapify(float* arr, int n, int i, int low) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[low + left] > arr[low + largest])
        largest = left;
    if (right < n && arr[low + right] > arr[low + largest])
        largest = right;
    if (largest != i) {
        swap(arr[low + i], arr[low + largest]);
        heapify(arr, n, largest, low);
    }
}

// Сортировка кучей (heapsort) для подмассива [low; high].
void heapsort(float* arr, int low, int high) {
    int n = high - low + 1;
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, low);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(arr[low], arr[low + i]);
        heapify(arr, i, 0, low);
    }
}

// Рекурсивная функция интроспективной сортировки.
// При достижении заданного предела глубины переключается на heapsort.
void introsort_util(float* arr, int low, int high, int depthLimit) {
    int n = high - low + 1;
    if (n < 16) {
        insertion_sort(arr, low, high);
        return;
    }
    if (depthLimit == 0) {
        heapsort(arr, low, high);
        return;
    }
    // Медиана трёх: выбираем опорный элемент.
    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[low])
        swap(arr[mid], arr[low]);
    if (arr[high] < arr[low])
        swap(arr[high], arr[low]);
    if (arr[high] < arr[mid])
        swap(arr[high], arr[mid]);
    // Перемещаем медиану в предпоследнюю позицию.
    swap(arr[mid], arr[high - 1]);
    float pivot = arr[high - 1];
    int i = low, j = high - 1;
    while (true) {
        while (arr[++i] < pivot) {}
        while (arr[--j] > pivot) {}
        if (i >= j)
            break;
        swap(arr[i], arr[j]);
    }
    swap(arr[i], arr[high - 1]); // Восстанавливаем опорный элемент
    introsort_util(arr, low, i - 1, depthLimit - 1);
    introsort_util(arr, i + 1, high, depthLimit - 1);
}

// Обёртка для интроспективной сортировки.
void introsort(float* arr, int n) {
    int depthLimit = 2 * (int)log2(n);
    introsort_util(arr, 0, n - 1, depthLimit);
}

/* ========= Конец интроспективной сортировки ========= */

// Функция заполнения всего массива случайными числами.
void random_fill_all(float* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (rand() % 1001) / 10.0f;
    }
    cout << "Массив заполнен случайными числами с одним знаком после запятой.\n";
}

// Функция случайного заполнения выбранных позиций в массиве.
void random_fill_positions(float* arr, int n, int positions) {
    if (positions > n)
        positions = n;
    int* indices = new int[n];
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }
    for (int i = 0; i < positions; i++) {
        int idx = indices[i];
        arr[idx] = (rand() % 1001) / 10.0f;
    }
    delete[] indices;
    cout << "Случайное заполнение выполнено для " << positions << " позиций.\n";
}

int main() {
    setlocale(LC_ALL, "Rus");
    srand(static_cast<unsigned int>(time(NULL)));

    int arr_size = 0;
    float* arr = nullptr;
    bool arrayInitialized = false;
    int choice = 0;

    do {
        clear_console();
        cout << "\n=== Главное меню ===\n";
        cout << "1. Ввести новый массив\n";
        cout << "2. Вывести текущий массив\n";
        cout << "3. Линейный поиск (неотсортированный массив)\n";
        cout << "4. Сортировка массива методом пузырьковой сортировки (простой метод)\n";
        cout << "5. Сортировка массива методом интроспективной сортировки (быстрая сортировка)\n";
        cout << "6. Бинарный поиск (на отсортированном массиве)\n";
        cout << "7. Случайное заполнение массива выбранных позиций\n";
        cout << "8. Выход\n";
        choice = getValidatedInput<int>("Введите ваш выбор (или 0 для отмены операции): ");
        if (choice == 0) {
            cout << "Операция отменена.\n";
            continue;
        }
        switch (choice) {
        case 1: {
            int size = getValidatedInput<int>("\nВведите размер массива (или 0 для отмены): ");
            if (size == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            if (size < 0) {
                cout << "Размер массива должен быть положительным.\n";
                break;
            }
            if (arr != nullptr) {
                delete[] arr;
            }
            arr_size = size;
            arr = new float[arr_size];
            char randomChoice = getValidatedInput<char>("\nЗаполнить массив случайными числами? (Y/N, 0 для отмены): ");
            if (randomChoice == '0') {
                cout << "Операция отменена.\n";
                break;
            }
            if (randomChoice == 'Y' || randomChoice == 'y') {
                random_fill_all(arr, arr_size);
            }
            else {
                input_array(arr, arr_size);
            }
            arrayInitialized = true;
            break;
        }
        case 2: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
            }
            else {
                if (arr_size > 10000) {
                    if (getConfirmation("Вы точно уверены, что хотите вывести текущий массив? (Y/N): ")) {
                        output_array(arr, arr_size);
                    }
                    else {
                        cout << "Вывод массива отменён.\n";
                    }
                }
                else {
                    output_array(arr, arr_size);
                }
            }
            system("pause");
            break;
        }
        case 3: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            float target = getValidatedInput<float>("\nВведите значение для линейного поиска (или 0 для отмены): ");
            if (target == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            bool printPositions = true;
            if (arr_size > 10000) {
                printPositions = getConfirmation("\nВы точно уверены что хотите провести поиск? Результат может долго печататься (Y/N): ");
            }
            auto start = high_resolution_clock::now();
            linear_search(arr, arr_size, target, printPositions);
            auto end = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end - start).count();
            float duration_ms = duration / 1000.0f;
            cout << "\nВремя выполнения линейного поиска: " << duration << " мкс (" << duration_ms << " мс)\n";
            system("pause");
            break;
        }
        case 4: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
            }
            else {
                auto start = high_resolution_clock::now();
                bubble_sort(arr, arr_size);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start).count();
                float duration_ms = duration / 1000.0f;
                cout << "\nМассив отсортирован методом пузырьковой сортировки.\n";
                cout << "Время сортировки: " << duration << " мкс (" << duration_ms << " мс)\n";
            }
            system("pause");
            break;
        }
        case 5: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
            }
            else {
                auto start = high_resolution_clock::now();
                introsort(arr, arr_size);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(end - start).count();
                float duration_ms = duration / 1000.0f;
                cout << "\nМассив отсортирован методом интроспективной сортировки.\n";
                cout << "Время сортировки: " << duration << " мкс (" << duration_ms << " мс)\n";
            }
            system("pause");
            break;
        }
        case 6: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала введите массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            float target = getValidatedInput<float>("\nВведите значение для бинарного поиска (или 0 для отмены): ");
            if (target == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            cout << "\nПредполагается, что массив отсортирован по возрастанию.\n";
            bool printPosition = true;
            if (arr_size > 10000) {
                printPosition = getConfirmation("\nВы точно уверены что хотите провести поиск? Результат может долго печататься (Y/N): ");
            }
            auto start = high_resolution_clock::now();
            if (printPosition) {
                int index = binary_search(arr, arr_size, target);
                auto end = high_resolution_clock::now();
                if (index != -1) {
                    cout << "Элемент " << target << " найден на позиции " << index << ".\n";
                }
                else {
                    cout << "Элемент " << target << " не найден в массиве.\n";
                }
                auto duration = duration_cast<microseconds>(end - start).count();
                float duration_ms = duration / 1000.0f;
                cout << "\nВремя выполнения бинарного поиска: " << duration << " мкс (" << duration_ms << " мс)\n";
            }
            else {
                int count = binary_search_count(arr, arr_size, target);
                auto end = high_resolution_clock::now();
                if (count > 0)
                    cout << "Элемент " << target << " найден " << count << " раз(а).\n";
                else
                    cout << "Элемент " << target << " не найден в массиве.\n";
                auto duration = duration_cast<microseconds>(end - start).count();
                float duration_ms = duration / 1000.0f;
                cout << "\nВремя выполнения бинарного поиска: " << duration << " мкс (" << duration_ms << " мс)\n";
            }
            system("pause");
            break;
        }
        case 7: {
            if (!arrayInitialized) {
                cout << "\nМассив не введён. Сначала создайте массив (пункт 1 меню).\n";
                system("pause");
                break;
            }
            int positions = getValidatedInput<int>("\nВведите количество позиций для случайного заполнения (или 0 для отмены): ");
            if (positions == 0) {
                cout << "Операция отменена.\n";
                break;
            }
            if (positions > arr_size) {
                positions = arr_size;
                cout << "Введенное количество превышает размер массива. Будут заполнены все позиции.\n";
            }
            random_fill_positions(arr, arr_size, positions);
            system("pause");
            break;
        }
        case 8: {
            cout << "\nВыход из программы. До свидания!\n";
            break;
        }
        default:
            cout << "\nНекорректный выбор. Попробуйте снова.\n";
            system("pause");
            break;
        }
    } while (choice != 8);

    if (arr != nullptr)
        delete[] arr;

    return 0;
}
