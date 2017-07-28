#ifndef __LGNUNIX_API_H__
#define __LGNUNIX_API_H__


#ifndef LGN_SYSTEMTIME_DECLARE
#define LGN_SYSTEMTIME_DECLARE
typedef struct _SYSTEMTIME {
    UINT16 wYear;
    UINT16 wMonth;
    UINT16 wDayOfWeek;
    UINT16 wDay;
    UINT16 wHour;
    UINT16 wMinute;
    UINT16 wSecond;
    UINT16 wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;
#endif

namespace LGN
{
#if (LGN_SUPPORT_MULTITHREAD)
	extern void LgnTheadSafe_Enter();
	extern void LgnTheadSafe_Leave();
#endif

	namespace API
	{
		LGN_INLINE HRESULT HResultFromError(INT32 nError)
		{
			return (nError <= 0) ? (HRESULT)nError : (HRESULT) (nError | 0x80000000);
		}

		// Memory
		LGN_INLINE void memcpy(LPVOID pDest, LPCVOID pSrc, size_t nSize)
		{
			::memcpy(pDest, pSrc, nSize);
		}
		LGN_INLINE void memmove(LPVOID pDest, LPCVOID pSrc, size_t nSize)
		{
			::memmove(pDest, pSrc, nSize);
		}
		LGN_INLINE int memcmp(LPCVOID pBuff1, LPCVOID pBuff2, size_t nSize)
		{
			return ::memcmp(pBuff1, pBuff2, nSize);
		}		
		LGN_INLINE void memset(LPVOID pDest, BYTE btFill, size_t nSize)
		{
			::memset(pDest, btFill, nSize);
		}
		LGN_INLINE void memzero(LPVOID pDest, size_t nSize)
		{
			::memset(pDest, 0, nSize);
		}

		// String
		LGN_INLINE int SafeStringLength(LPCSTR pszStr)
		{
			return (pszStr == NULL)? 0 : (int)::strlen(pszStr);
		}
		LGN_INLINE int SafeStringLength(LPCWSTR pszStr)
		{
			int i=0;
			if (pszStr != NULL)
				for (; pszStr[i]; i++) ;
			return i;
		}
		
		LGN_INLINE ULONG SafeIncrement(ULONG * pVal)
		{
#if (LGN_SUPPORT_MULTITHREAD)
			LgnTheadSafe_Enter();
			ULONG ul = ++(*pVal);
			LgnTheadSafe_Leave();
			return ul;
#else
			return ++(*pVal);
#endif
		}
		LGN_INLINE ULONG SafeDecrement(ULONG * pVal)
		{
#if (LGN_SUPPORT_MULTITHREAD)
			LgnTheadSafe_Enter();
			ULONG ul = --(*pVal);
			LgnTheadSafe_Leave();
			return ul;
#else
			return --(*pVal);
#endif
		}
		LGN_INLINE ULONG SafeExchange(ULONG* pVal,ULONG Value)
		{
#if (LGN_SUPPORT_MULTITHREAD)
			LgnTheadSafe_Enter;
			(*pVal) = Value;
			LgnTheadSafe_Leave;
#else
			(*pVal) = Value;
#endif
			return 1;
		}

		LGN_INLINE void CloseHandle(HANDLE hObject)
		{
		}

		LGN_INLINE void Sleep(UINT32 dwMilliseconds)
		{
			::usleep(dwMilliseconds * 1000);
		}
		LGN_INLINE UINT32 GetTickCount()
		{
			time_t now;
			return (UINT32)::time(&now);
		}
		LGN_INLINE void GetLocalTime(LPSYSTEMTIME lpSystemTime)
		{
			time_t now;
			struct tm *timenow;

			::time(&now);
			timenow = ::localtime(&now);

			lpSystemTime->wYear = 1900 + timenow->tm_year;
			lpSystemTime->wMonth = timenow->tm_mon + 1;
			lpSystemTime->wDayOfWeek = timenow->tm_wday;
			lpSystemTime->wDay = timenow->tm_mday;
			lpSystemTime->wHour = timenow->tm_hour;
			lpSystemTime->wMinute = timenow->tm_min;
			lpSystemTime->wSecond = timenow->tm_sec;
			lpSystemTime->wMilliseconds = 0;
		}

		LGN_INLINE PID_T GetCurrentProcessId()
		{
#ifdef _UNISTD_H
			return (PID_T)getpid();
#else
			return NULL;
#endif
		}
		LGN_INLINE TID_T GetCurrentThreadId()
		{
#ifdef _PTHREAD_H
			return (TID_T)pthread_self();
#else
			return NULL;
#endif
		}
				
	}; // namespace API

}; // namespace LGN

#endif // #ifndef __LGNUNIX_API_H__
