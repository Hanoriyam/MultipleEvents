#include "MultipleEvents.h"

MultipleEvents::Thread::Thread() :
	m_bIsRunning{ false }
{
}

MultipleEvents::Thread::~Thread()
{
	Stop();
	ClearEventHandles();
}

bool MultipleEvents::Thread::CreateGlobalEvent(const std::string_view svEventName)
{
	SECURITY_DESCRIPTOR sd{ 0, };
	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		return false;
	}

	if (!SetSecurityDescriptorDacl(&sd, true, nullptr, false))
	{
		return false;
	}

	SECURITY_ATTRIBUTES sa{ 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = false;
	sa.lpSecurityDescriptor = &sd;

	std::string strGlobalEvent;
	strGlobalEvent += "Global\\";
	strGlobalEvent += svEventName.data();
	if (!this->CreateGlobalEventA(&sa, false, false, strGlobalEvent.c_str()))
	{
		return false;
	}

	return true;
}

bool MultipleEvents::Thread::CreateGlobalEvent(const std::wstring_view wsvEventName)
{
	SECURITY_DESCRIPTOR sd{ 0, };
	if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
	{
		return false;
	}

	if (!SetSecurityDescriptorDacl(&sd, true, nullptr, false))
	{
		return false;
	}

	SECURITY_ATTRIBUTES sa{ 0 };
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = false;
	sa.lpSecurityDescriptor = &sd;

	std::wstring wstrGlobalEvent;
	wstrGlobalEvent += L"Global\\";
	wstrGlobalEvent += wsvEventName.data();
	if (!this->CreateGlobalEventW(&sa, false, false, wstrGlobalEvent.c_str()))
	{
		return false;
	}

	return true;
}

bool MultipleEvents::Thread::CreateGlobalEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
	HANDLE hEvent{ ::CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName) };
	if (hEvent == nullptr)
	{
		return false;
	}

	std::string strGlobalEventSuccessName{ lpName };
	strGlobalEventSuccessName += "_S";
	HANDLE hEventSuccess{ ::CreateEventA(lpEventAttributes, false, false, strGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::CreateGlobalEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCWSTR lpName)
{
	HANDLE hEvent{ ::CreateEventW(lpEventAttributes, bManualReset, bInitialState, lpName) };
	if (hEvent == nullptr)
	{
		return false;
	}

	std::wstring wstrGlobalEventSuccessName{ lpName };
	wstrGlobalEventSuccessName += L"_S";
	HANDLE hEventSuccess{ ::CreateEventW(lpEventAttributes, false, false, wstrGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::OpenGlobalEvent(const std::string_view svEventName)
{
	std::string strEventName;
	strEventName += "Global\\";
	strEventName += svEventName;

	HANDLE hEvent{ ::OpenEventA(READ_CONTROL | EVENT_MODIFY_STATE, false, strEventName.c_str()) };
	if (hEvent == nullptr)
	{
		return false;
	}

	std::string strGlobalEventSuccessName{ strEventName };
	strGlobalEventSuccessName += "_S";
	HANDLE hEventSuccess{ ::OpenEventA(READ_CONTROL | EVENT_MODIFY_STATE, false, strGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::OpenGlobalEvent(const std::wstring_view wsvEventName)
{
	std::wstring wstrEventName;
	wstrEventName += L"Global\\";
	wstrEventName += wsvEventName;

	HANDLE hEvent{ ::OpenEventW(READ_CONTROL | EVENT_MODIFY_STATE, false, wstrEventName.c_str()) };
	if (hEvent == nullptr)
	{
		return false;
	}

	std::wstring wstrGlobalEventSuccessName{ wstrEventName };
	wstrGlobalEventSuccessName += L"_S";
	HANDLE hEventSuccess{ ::OpenEventW(READ_CONTROL | EVENT_MODIFY_STATE, false, wstrGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::CreateEventA()
{
	HANDLE hEvent{ ::CreateEventA(nullptr, false, false, nullptr) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::CreateEventA(nullptr, false, false, nullptr) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::CreateEventW()
{
	HANDLE hEvent{ ::CreateEventW(nullptr, false, false, nullptr) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::CreateEventW(nullptr, false, false, nullptr) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::CreateEventA(
	LPSECURITY_ATTRIBUTES lpEventAttributes,
	BOOL bManualReset,
	BOOL bInitialState,
	LPCSTR lpName)
{
	HANDLE hEvent{ ::CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::CreateEventA(nullptr, false, false, nullptr) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

bool MultipleEvents::Thread::CreateEventW(
	LPSECURITY_ATTRIBUTES lpEventAttributes,
	BOOL bManualReset,
	BOOL bInitialState,
	LPCWSTR lpName)
{
	HANDLE hEvent{ ::CreateEventW(lpEventAttributes, bManualReset, bInitialState, lpName) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::CreateEventW(nullptr, false, false, nullptr) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	m_mapEventSuccess[hEvent] = hEventSuccess;
	m_arrTargetEvent.push_back(std::move(hEvent));
	m_arrTargetEventSuccess.push_back(std::move(hEventSuccess));

	return true;
}

void MultipleEvents::Thread::RegisterQueueProcessFunc(std::function<void(DWORD)> funcQueueProcess)
{
	std::function<void(DWORD)> func{
		[&, funcQueueProcess](DWORD dwEventIndex)
		{
			funcQueueProcess(dwEventIndex);

			if (m_mapEventSuccess.find(m_arrTargetEvent[dwEventIndex]) == m_mapEventSuccess.end())
			{
				return;
			}

			if (!::SetEvent(m_mapEventSuccess[m_arrTargetEvent[dwEventIndex]]))
			{
				printf("RegisterQueueProcessFunc error. (%d)\n", GetLastError());
			}
		} };

	m_threadQueueProcess.RegisterQueueProcessFunc(std::move(func));
}

bool MultipleEvents::Thread::Start()
{
	if (m_bIsRunning)
		return false;

	size_t stThreadCount{
		static_cast<size_t>(
			ceil(static_cast<double>(m_arrTargetEvent.size()) /
				static_cast<double>((MAXIMUM_WAIT_OBJECTS - 1))))
	};

	ClearExitEventHandles();
	for (size_t i{ 0 }; i < stThreadCount; ++i)
	{
		HANDLE hExitEvent{ ::CreateEvent(nullptr, false, false, nullptr) };
		if (hExitEvent == nullptr)
		{
			return false;
		}
		m_arrExitEvent.push_back(hExitEvent);
	}

	std::vector<HANDLE> arrHandles;
	arrHandles.reserve(m_arrExitEvent.size() + m_arrTargetEvent.size());

	size_t stPushCount{ 0 };
	for (const auto& hExitEvent : m_arrExitEvent)
	{
		arrHandles.push_back(hExitEvent);
		for (size_t j{ 0 }; j < 63 && stPushCount < m_arrTargetEvent.size(); ++j)
		{
			arrHandles.push_back(m_arrTargetEvent[stPushCount++]);
		}
	}
	m_arrHandles.swap(arrHandles);

	m_arrWaitThreads.reserve(stThreadCount);
	for (size_t i{ 0 }; i < stThreadCount; ++i)
	{
		size_t stWaitSize{
			((i != stThreadCount - 1) ?
				MAXIMUM_WAIT_OBJECTS : m_arrHandles.size() - (i * MAXIMUM_WAIT_OBJECTS))
		};

		m_arrWaitThreads.push_back(
			std::thread(
				&MultipleEvents::Thread::WaitThreadRun,
				this,
				stWaitSize,
				m_arrHandles.data() + (MAXIMUM_WAIT_OBJECTS * i),
				i
			));
	}

	bool bIsAllThreadStart{ false };
	do
	{
		if (!StartQueueProcessThread())
		{
			break;
		}

		if (!StartEventSignalThread())
		{
			break;
		}
		bIsAllThreadStart = true;
	} while (false);

	if (bIsAllThreadStart == false)
	{
		m_threadQueueProcess.Stop();
		m_threadEventSignal.Stop();
		return false;
	}

	if (!GetCurrentEventHandleAddressMap())
	{
		return false;
	}

	if (m_mapHandleAddress.size() == m_arrTargetEvent.size())
	{
		return false;
	}

	m_bIsRunning = true;

	return true;
}

bool MultipleEvents::Thread::Start(std::function<void(DWORD)> funcQueueProcess)
{
	RegisterQueueProcessFunc(funcQueueProcess);
	return Start();
}

void MultipleEvents::Thread::Stop()
{
	if (!m_bIsRunning)
		return;

	WaitThreadStop();

	m_bIsRunning = false;
}

bool MultipleEvents::Thread::SetEvent(DWORD dwEventIndex)
{
	if (!m_bIsRunning)
	{
		return false;
	}

	if (dwEventIndex < 0 || dwEventIndex >= m_arrTargetEvent.size())
	{
		return false;
	}

	m_threadEventSignal.AddQueueData(m_arrTargetEvent[dwEventIndex]);

	return true;
}

size_t MultipleEvents::Thread::GetEventCount() const
{
	return m_arrTargetEvent.size();
}

void MultipleEvents::Thread::WaitThreadRun(DWORD dwSize, const HANDLE* pHandle, DWORD dwThreadOffset)
{
	bool bThreadExit{ false };
	while (!bThreadExit)
	{
		DWORD dwWait{
			WaitForMultipleObjects(
				dwSize,
				pHandle,
				false,
				INFINITE) };

		switch (dwWait)
		{
			case WAIT_FAILED:
			{
				bThreadExit = true;
				break;
			}

			case WAIT_ABANDONED:
			{
				bThreadExit = true;
				break;
			}

			case WAIT_OBJECT_0:
			{
				bThreadExit = true;
				break;
			}

			default:
			{
				if (dwWait >= WAIT_OBJECT_0 + 1 &&
					dwWait <= WAIT_OBJECT_0 + dwSize)
				{
					DWORD dwWaitIndex{ dwThreadOffset * (MAXIMUM_WAIT_OBJECTS - 1) + dwWait - 1 };
					m_threadQueueProcess.AddQueueData(dwWaitIndex);
				}

				break;
			}
		}
	}
}

void MultipleEvents::Thread::WaitThreadStop()
{
	for (const auto& hExitEvent : m_arrExitEvent)
	{
		::SetEvent(hExitEvent);
	}
	ClearExitEventHandles();
	std::this_thread::sleep_for(200ms);

	for (auto& thread : m_arrWaitThreads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
	m_arrWaitThreads.clear();
}

void MultipleEvents::Thread::ClearExitEventHandles()
{
	for (auto& hExitEvent : m_arrExitEvent)
	{
		if (hExitEvent) CloseHandle(hExitEvent);
		hExitEvent = nullptr;
	}
	m_arrExitEvent.clear();
}

void MultipleEvents::Thread::ClearEventHandles()
{
	for (const auto& hEvent : m_arrTargetEvent)
	{
		if (hEvent) CloseHandle(hEvent);
	}
	m_arrTargetEvent.clear();

	for (const auto& hEvent : m_arrTargetEventSuccess)
	{
		if (hEvent) CloseHandle(hEvent);
	}
	m_arrTargetEventSuccess.clear();
}

bool MultipleEvents::Thread::IsRunning() const
{
	return m_bIsRunning;
}

bool MultipleEvents::Thread::StartQueueProcessThread()
{
	return m_threadQueueProcess.Start();
}

void MultipleEvents::Thread::StopQueueProcessThread()
{
	m_threadQueueProcess.Stop();
}

bool MultipleEvents::Thread::StartEventSignalThread()
{
	return m_threadEventSignal.Start([&](HANDLE hEvent)
	{
		if (!::SetEvent(hEvent))
		{
			return;
		}

		if (m_mapEventSuccess.find(hEvent) == m_mapEventSuccess.end())
		{
			return;
		}

		WaitForSingleObject(m_mapEventSuccess[hEvent], WAIT_TIME);
	});
}

void MultipleEvents::Thread::StopEventSignalThread()
{
	m_threadEventSignal.Stop();
}

bool MultipleEvents::Thread::GetCurrentEventHandleAddressMap()
{
	m_mapHandleAddress.clear();

	typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
	{
		USHORT UniqueProcessId;
		USHORT CreatorBackTraceIndex;
		UCHAR ObjectTypeIndex;
		UCHAR HandleAttributes;
		USHORT HandleValue;
		PVOID Object;
		ULONG GrantedAccess;
	} SYSTEM_HANDLE_TABLE_ENTRY_INFO, * PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{
		ULONG NumberOfHandles;
		SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
	} SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

	size_t handleAddress{ 0 };

	HMODULE hDll{ GetModuleHandle(L"ntdll.dll") };
	if (hDll == nullptr)
	{
		return false;
	}

	auto pHandleTableInformation{
		reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(
			malloc(1024 * 1024 * 32))
	};

	if (pHandleTableInformation == nullptr)
	{
		return false;
	}

	ULONG ulReturnLength{ 0 };
	NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS(0x10), pHandleTableInformation, 1024 * 1024 * 32, &ulReturnLength);

	DWORD dwProcessId{ GetCurrentProcessId() };
	for (int i{ 0 }; i < pHandleTableInformation->NumberOfHandles; ++i)
	{
		auto handleInfo{ pHandleTableInformation->Handles[i] };
		if (handleInfo.UniqueProcessId != dwProcessId)
		{
			continue;
		}

		if (handleInfo.HandleValue == 0 || handleInfo.Object == 0)
		{
			continue;
		}

		if (handleInfo.ObjectTypeIndex != 0x10)
		{
			continue;
		}

		m_mapHandleAddress[
			reinterpret_cast<HANDLE>(handleInfo.HandleValue)] =
			reinterpret_cast<size_t>(handleInfo.Object);
	}

	if (pHandleTableInformation)
		free(pHandleTableInformation);

	return true;
}

bool MultipleEvents::SetGlobalEvent(const std::string_view svEventName)
{
	std::string strGlobalEventName{ "Global\\" };
	strGlobalEventName += svEventName;

	std::string strGlobalEventSuccessName{ strGlobalEventName };
	strGlobalEventSuccessName += "_S";

	HANDLE hEvent{ ::OpenEventA(EVENT_ALL_ACCESS, false, strGlobalEventName.c_str()) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::OpenEventA(EVENT_ALL_ACCESS, false, strGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	if (!::SetEvent(hEvent))
	{
		return false;
	}

	DWORD dwWait{ WaitForSingleObject(hEventSuccess, WAIT_TIME) };
	if (dwWait == WAIT_FAILED || dwWait == WAIT_ABANDONED)
	{
		Sleep(WAIT_TIME_FOR_FAIL);
	}

	if (hEvent) CloseHandle(hEvent);
	if (hEventSuccess) CloseHandle(hEventSuccess);

	return true;
}

bool MultipleEvents::SetGlobalEvent(const std::wstring_view wsvEventName)
{
	std::wstring wstrGlobalEventName{ L"Global\\" };
	wstrGlobalEventName += wsvEventName;

	std::wstring wstrGlobalEventSuccessName{ wstrGlobalEventName };
	wstrGlobalEventSuccessName += L"_S";

	HANDLE hEvent{ ::OpenEventW(EVENT_ALL_ACCESS, false, wstrGlobalEventName.c_str()) };
	if (hEvent == nullptr)
	{
		return false;
	}

	HANDLE hEventSuccess{ ::OpenEventW(EVENT_ALL_ACCESS, false, wstrGlobalEventSuccessName.c_str()) };
	if (hEventSuccess == nullptr)
	{
		return false;
	}

	if (!::SetEvent(hEvent))
	{
		return false;
	}

	DWORD dwWait{ WaitForSingleObject(hEventSuccess, WAIT_TIME) };
	if (dwWait == WAIT_FAILED || dwWait == WAIT_ABANDONED)
	{
		Sleep(WAIT_TIME_FOR_FAIL);
	}

	if (hEvent) CloseHandle(hEvent);
	if (hEventSuccess) CloseHandle(hEventSuccess);

	return true;
}