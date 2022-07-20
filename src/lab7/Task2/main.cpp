#include <Windows.h>

#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>

#include "Utils/process.h"

namespace {

constexpr DWORD cDefaultThreadIdleTimeMs = 800;

std::atomic<DWORD> gExecutionTimes = {};
std::mutex outputLock;

DWORD ThreadRoutine(LPVOID userData) {
  const auto threadNo = *static_cast<DWORD *>(userData);
  gExecutionTimes++;

  outputLock.lock();  // guard shared resource
  std::cout << "Begin thread: " << threadNo << std::endl;
  outputLock.unlock();  // keep lock for smallest time possible

  Sleep(cDefaultThreadIdleTimeMs);  // imitate doing hard computations

  outputLock.lock();
  std::cout << "End thread: " << threadNo << std::endl;
  outputLock.unlock();

  return 0;
}

}  // namespace

int main() {
  constexpr int cThreadAmount = 10;
  DWORD threadRoutineArgs[cThreadAmount];
  HANDLE threadHandles[cThreadAmount];

  for (int i = 0; i < cThreadAmount; ++i) {
    threadRoutineArgs[i] = i + 1;
    threadHandles[i] =
        CreateThread(nullptr,                // default security attrs
                     0,                      // default stack size
                     ThreadRoutine,          // thread function
                     &threadRoutineArgs[i],  // function parameter
                     0,       // flags, 0 - run immediately after creation
                     nullptr  // out ptr to thread id, ignored
        );
    if (!threadHandles[i]) {
      lab7::HandleFatalWinApiError("Create thread failed");
    }
  }

  if (WaitForMultipleObjects(std::size(threadHandles), threadHandles, false,
                             INFINITE) == WAIT_FAILED) {
    lab7::HandleFatalWinApiError("WaitForMultipleObjects failed");
  }

  const DWORD timesExecuted = gExecutionTimes.load();

  outputLock.lock();
  std::cout << "Thread routine has been executed" << ' ' << timesExecuted << ' '
            << "times" << std::endl;
  outputLock.unlock();

  //  Wait for all threads to finish
  WaitForMultipleObjects(cThreadAmount, threadHandles, true, INFINITE);

  return EXIT_SUCCESS;
}
