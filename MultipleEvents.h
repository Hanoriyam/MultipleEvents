#pragma once
#include <Windows.h>
#include <winternl.h>
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <functional>
#include <map>
#include <string_view>
#include "QueueThread.h"
using namespace std::chrono_literals;

#pragma comment(lib, "ntdll")

namespace MultipleEvents
{
	constexpr unsigned int WAIT_TIME{ 1000 };
	constexpr unsigned int WAIT_TIME_FOR_FAIL{ 10 };
	class Thread
	{
	public:
		Thread();
		~Thread();

	public:
		bool CreateGlobalEvent(const std::string_view svEventName);
		bool CreateGlobalEvent(const std::wstring_view wsvEventName);
		bool CreateGlobalEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
		bool CreateGlobalEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName);
		bool OpenGlobalEvent(const std::string_view svEventName);
		bool OpenGlobalEvent(const std::wstring_view wsvEventName);

		bool CreateEventA();
		bool CreateEventW();
		bool CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName);
		bool CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName);

	public:
		void RegisterQueueProcessFunc(std::function<void(DWORD)> funcQueueProcess);
		bool Start();
		bool Start(std::function<void(DWORD)> funcQueueProcess);
		void Stop();
		bool SetEvent(DWORD dwEventIndex);
		size_t GetEventCount() const;

	private:
		void WaitThreadRun(DWORD dwSize, const HANDLE* pHandle, DWORD dwThreadOffset);
		void WaitThreadStop();

		void ClearExitEventHandles();
		void ClearEventHandles();

		bool IsRunning() const;

		bool StartQueueProcessThread();
		void StopQueueProcessThread();

		bool StartEventSignalThread();
		void StopEventSignalThread();

	private:
		bool GetCurrentEventHandleAddressMap();
		size_t GetHandleAddress(const HANDLE& handle) const;

	private:
		bool m_bIsRunning;

		std::vector<HANDLE> m_arrExitEvent;
		std::vector<HANDLE> m_arrTargetEvent;
		std::vector<HANDLE> m_arrTargetEventSuccess;
		std::vector<HANDLE> m_arrHandles;

		std::map<HANDLE, HANDLE> m_mapEventSuccess;
		std::map<HANDLE, size_t> m_mapHandleAddress;

		std::vector<std::thread> m_arrWaitThreads;

		QueueThread<DWORD> m_threadQueueProcess;
		QueueThread<HANDLE> m_threadEventSignal;
	};

	bool SetGlobalEvent(const std::string_view svEventName);
	bool SetGlobalEvent(const std::wstring_view wsvEventName);
}