// main.cpp
#include "ArrayManager.h"
#include "ThreadManager.h"
#include "MarkerLogic.h"
#include <iostream>
#include <windows.h>

void marker_logic(int id, ArrayManager& arr) {
    srand(static_cast<unsigned>(id + time(nullptr)));
    int r = rand();
    int idx = r % arr.size();
    if (idx < 0) idx = -idx;

    if (arr[idx] != 0 && (arr[idx] % arr.size() == 0)) {
        Sleep(5);
        arr[idx] = id;
        Sleep(5);
        return;
    }

    int marked = arr.count_marked();
    std::cout << "[marker " << id << "] "
        << "свой порядковый номер: " << id << ", "
        << "количество помеченных элементов: " << marked << ", "
        << "индекс элемента массива, который невозможно пометить: " << idx << "\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int size, count;
    std::cout << "Размер массива: "; std::cin >> size;
    std::cout << "Количество потоков: "; std::cin >> count;

    ArrayManager arr(size);
    ThreadManager tm(count, marker_logic, arr);
    tm.start_all();

    while (true) {
        tm.wait_all_blocked();
        std::cout << arr.to_string();

        std::cout << "Завершить поток (1.." << count << ") или 0: ";
        int chosen; std::cin >> chosen;
        if (chosen == 0) {
            tm.start_all();
            continue;
        }
        if (chosen < 1 || chosen > count) continue;

        tm.terminate(chosen);
        std::cout << arr.to_string();

        tm.start_all();
        if (tm.all_blocked()) break;
    }

    std::cout << "Готово.\n";
    system("pause");
    return 0;
}