// MarkerLogic.cpp
#include "MarkerLogic.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

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