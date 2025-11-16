// ArrayManager.h
#pragma once
#include <vector>
#include <string>

class ArrayManager {
public:
    explicit ArrayManager(int size);
    ~ArrayManager();

    int size() const { return size_; }
    int& operator[](int idx) { return data_[idx]; }
    const int& operator[](int idx) const { return data_[idx]; }

    int count_marked() const;
    void clear_marker(int marker_id);
    std::string to_string() const;

private:
    int size_;
    std::vector<int> data_;
};