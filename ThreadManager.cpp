// ThreadManager.cpp
#include "ThreadManager.h"
#include <iostream>
#include <stdexcept>

ThreadManager::ThreadManager(int count, MarkerFunc func, ArrayManager& array)
    : array_(array)
{
    threads_.reserve(count);
    for (int i = 0; i < count; ++i) {
        ThreadData td{};
        td.id = i + 1;
        td.array = &array_;
        td.func = func;

        td.hStart = CreateEventA(nullptr, TRUE, FALSE, nullptr);
        td.hDone = CreateEventA(nullptr, FALSE, FALSE, nullptr);
        td.hTerm = CreateEventA(nullptr, FALSE, FALSE, nullptr);

        if (!td.hStart || !td.hDone || !td.hTerm) {
            throw std::runtime_error("Failed to create event");
        }

        // Добавляем элемент в вектор
        threads_.emplace_back(td);

        // Берём ссылку на только что добавленный элемент
        ThreadData& thread_data = threads_.back();

        // Создаём поток, передавая указатель на этот элемент
        thread_data.hThread = CreateThread(
            nullptr, 0, ThreadProc, &thread_data, 0, nullptr);

        if (!thread_data.hThread) {
            throw std::runtime_error("Failed to create thread");
        }
    }
}

ThreadManager::~ThreadManager() {
    for (auto& t : threads_) {
        if (t.hThread) {
            SetEvent(t.hTerm);
            WaitForSingleObject(t.hThread, INFINITE);
            CloseHandle(t.hThread);
        }
        if (t.hStart) CloseHandle(t.hStart);
        if (t.hDone)  CloseHandle(t.hDone);
        if (t.hTerm)  CloseHandle(t.hTerm);
    }
}

void ThreadManager::start_all() {
    for (auto& t : threads_) {
        if (t.hThread) SetEvent(t.hStart);
    }
}

void ThreadManager::signal_continue(int id) {
    for (auto& t : threads_) {
        if (t.id == id && t.hThread) {
            SetEvent(t.hStart);
            break;
        }
    }
}

void ThreadManager::terminate(int id) {
    for (auto& t : threads_) {
        if (t.id == id && t.hThread) {
            SetEvent(t.hTerm);
            WaitForSingleObject(t.hThread, INFINITE);
            CloseHandle(t.hThread);
            t.hThread = nullptr;
            std::cout << "[main] Поток marker " << id << " завершён\n";
            return;
        }
    }
}

bool ThreadManager::all_blocked() {
    for (const auto& t : threads_) {
        if (t.hThread && WaitForSingleObject(t.hDone, 0) != WAIT_OBJECT_0)
            return false;
    }
    return true;
}

void ThreadManager::wait_all_blocked() {
    std::vector<HANDLE> done_events;
    for (const auto& t : threads_)
        if (t.hThread) done_events.push_back(t.hDone);

    if (!done_events.empty()) {
        WaitForMultipleObjects(
            static_cast<DWORD>(done_events.size()),
            done_events.data(), TRUE, INFINITE);

        for (auto h : done_events) ResetEvent(h);
    }
}

DWORD WINAPI ThreadManager::ThreadProc(LPVOID param) {
    ThreadData* td = static_cast<ThreadData*>(param);
    HANDLE handles[] = { td->hStart, td->hTerm };

    while (true) {
        DWORD wait = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
        if (wait == WAIT_OBJECT_0 + 1) break; // terminate
        if (wait == WAIT_OBJECT_0) {
            ResetEvent(td->hStart);
            td->func(td->id, *td->array);
            SetEvent(td->hDone); // сигнализируем, что не можем пометить
        }
    }
    return 0;
}