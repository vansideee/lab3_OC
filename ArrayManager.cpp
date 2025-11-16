// ArrayManager.cpp
#include "ArrayManager.h"
#include <iomanip>
#include <sstream>

ArrayManager::ArrayManager(int size) : size_(size), data_(size, 0) {}

ArrayManager::~ArrayManager() = default;

int ArrayManager::count_marked() const {
    int count = 0;
    for (int val : data_)
        if (val != 0) ++count;
    return count;
}

void ArrayManager::clear_marker(int marker_id) {
    for (int& val : data_)
        if (val == marker_id)
            val = 0;
}

std::string ArrayManager::to_string() const {
    std::ostringstream oss;
    oss << "\n=== Массив ===\n";
    for (int i = 0; i < size_; ++i) {
        oss << std::setw(4) << data_[i] << ' ';
        if ((i + 1) % 20 == 0) oss << '\n';
    }
    if (size_ % 20 != 0) oss << '\n';
    return oss.str();
}