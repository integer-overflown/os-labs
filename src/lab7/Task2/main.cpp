#include <Windows.h>

#include <algorithm>
#include <atomic>
#include <iostream>
#include <mutex>

namespace {

constexpr DWORD cDefaultThreadIdleTimeMs = 800;

std::atomic<DWORD> gExecutionTimes = {};
std::mutex outputLock;

void HandleFatalWinApiError(const char *reason) {
  std::cerr << reason << ", error code " << GetLastError() << '\n';
  std::terminate();
}

DWORD ThreadRoutine(LPVOID userData) {
  const auto threadNo = *static_cast<DWORD *>(userData);
  gExecutionTimes++;

  outputLock.lock();  // guard shared resource
  std::cout << "Begin thread: " << threadNo << '\n';
  outputLock.unlock();  // keep lock for smallest time possible

  Sleep(cDefaultThreadIdleTimeMs);  // imitate doing hard computations

  outputLock.lock();
  std::cout << "End thread: " << threadNo << '\n';
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
      HandleFatalWinApiError("Create thread failed");
    }
  }

  if (WaitForMultipleObjects(cThreadAmount, threadHandles, false, INFINITE) ==
      WAIT_FAILED) {
    HandleFatalWinApiError("WaitForMultipleObjects failed");
  }

  std::cout << "Thread routine has been executed" << ' '
            << gExecutionTimes.load() << ' ' << "times" << '\n';

  //  Wait for all threads to finish
  WaitForMultipleObjects(cThreadAmount, threadHandles, true, INFINITE);

  return EXIT_SUCCESS;
}
