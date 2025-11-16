// ThreadManager.h
#pragma once
#include "ArrayManager.h"
#include <windows.h>
#include <vector>
#include <functional>

class ThreadManager {
public:
    using MarkerFunc = std::function<void(int, ArrayManager&)>;

    ThreadManager(int count, MarkerFunc func, ArrayManager& array);
    ~ThreadManager();

    void start_all();
    void signal_continue(int id);
    void terminate(int id);
    bool all_blocked();
    void wait_all_blocked();

private:
    struct ThreadData {
        int id;
        HANDLE hStart, hDone, hTerm;
        HANDLE hThread;
        ArrayManager* array;
        MarkerFunc func;
    };

    static DWORD WINAPI ThreadProc(LPVOID param);

    std::vector<ThreadData> threads_;
    ArrayManager& array_;
};