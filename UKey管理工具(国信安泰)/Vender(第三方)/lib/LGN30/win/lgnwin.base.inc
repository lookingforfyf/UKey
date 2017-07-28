 
#if (!LGN_SUPPORT_SELECTANY)

namespace LGN
{
#if (LGN_SUPPORT_TRACE)
#ifdef __LGNTRACE_H__
	CTraceCategory lgnTraceGeneral(LGN_TRACE_CATEGORY_DEF_FLAGS, LGN_TRACE_CATEGORY_DEF_LEVEL, LGN_TRACE_CATEGORY_DEF_NAME, LGN_TRACE_CATEGORY_DEF_FILE);
#endif
#endif // #if (LGN_SUPPORT_TRACE)

#ifdef __LGNWIN_ALLOC_H__
	CWin32Heap g_heapmgr(::GetProcessHeap());
	ILgnMemMgr * _lgnMemoryManager = &g_heapmgr;

	CLgnBufferMgr g_bufmgr(_lgnMemoryManager);
	ILgnBufferMgr * _lgnBufferManager = &g_bufmgr;
#endif

#ifdef __LGNWIN_VER_H__
	COSVersionInfoEx g_osVersionInfo;
#endif

#ifdef __LGNWIN_AUTH_H__
	CSecurityAttributes g_saLowIntegrity(false);
#endif
};


#endif // #if (!LGN_SUPPORT_SELECTANY)


#ifdef _LGN_NO_DEFAULT_LIBS

// C‘À––ø‚£∫libcmt.LIB 

typedef void (*_PVFV)(void);
extern "C" _PVFV* __onexitbegin = NULL;
extern "C" _PVFV* __onexitend = NULL;


#define _CRTALLOC(x)

#pragma data_seg(".CRT$XIA")
_CRTALLOC(".CRT$XIA") _PVFV __xi_a[] = { NULL };

#pragma data_seg(".CRT$XIZ")
_CRTALLOC(".CRT$XIZ") _PVFV __xi_z[] = { NULL };

#pragma data_seg(".CRT$XCA")
_CRTALLOC(".CRT$XCA") _PVFV __xc_a[] = { NULL };

#pragma data_seg(".CRT$XCZ")
_CRTALLOC(".CRT$XCZ") _PVFV __xc_z[] = { NULL };

#pragma data_seg()  /* reset */


#if defined (_M_IA64)
#pragma comment(linker, "/merge:.CRT=.rdata")
#else  /* defined (_M_IA64) */
#pragma comment(linker, "/merge:.CRT=.data")
#endif  /* defined (_M_IA64) */



namespace LGN
{
	static HANDLE g_hProcessHeap = NULL;

	// Critical section to protect initialization/exit code
	static CRITICAL_SECTION g_csInit;


	static void DoInitialization()
	{
		::InitializeCriticalSection( &g_csInit );

		::EnterCriticalSection( &g_csInit );

#ifdef _LGN_SUPPORT_C_EXCEPT
		__try
#endif
		{
			g_hProcessHeap = ::GetProcessHeap();

			_PVFV* pf;
			// Call initialization routines (contructors for globals, etc.)
			for(pf = __xi_a; pf < __xi_z; pf++)
			{
				if(*pf != NULL)
				{
					(**pf)();
				}
			}

			for(pf = __xc_a; pf < __xc_z; pf++)
			{
				if(*pf != NULL)
				{
					(**pf)();
				}
			}
		}
#ifdef _LGN_SUPPORT_C_EXCEPT
		__finally
		{
			::LeaveCriticalSection(&g_csInit);
		}
#endif
	}

	static void DoCleanup()
	{
		::EnterCriticalSection(&g_csInit);  // Protect access to the atexit table

#ifdef _LGN_SUPPORT_C_EXCEPT
		__try
#endif
		{
			_PVFV* pf;
			// Call routines registered with atexit() from most recently registered
			// to least recently registered
			if(__onexitbegin != NULL)
			{
				for(pf = __onexitend-1; pf >= __onexitbegin; pf--)
				{
					if (*pf != NULL)
						(**pf)();
				}
			}

			::HeapFree( g_hProcessHeap, 0, __onexitbegin );
			__onexitbegin = NULL;
			__onexitend = NULL;

			//for(pf = __xp_a; pf < __xp_z; pf++)
			//{
			//	if( *pf != NULL )
			//	{
			//		(**pf)();
			//	}
			//}

			//for(pf = __xt_a; pf < __xt_z; pf++)
			//{
			//	if( *pf != NULL )
			//	{
			//		(**pf)();
			//	}
			//}
		}
#ifdef _LGN_SUPPORT_C_EXCEPT
		__finally
#endif		
		{
			::LeaveCriticalSection(&g_csInit);
			::DeleteCriticalSection(&g_csInit);		
		}
	}

}; // namespace LGN



void* __cdecl operator new(size_t n)
{
	return ::HeapAlloc( LGN::g_hProcessHeap, 0, n );
}

void* __cdecl operator new[](size_t n)
{
	return operator new(n);
}

void __cdecl operator delete(void* p)
{
	if (p != NULL)
		::HeapFree( LGN::g_hProcessHeap, 0, p );
}

void __cdecl operator delete[](void* p)
{
	operator delete(p);
}

#ifdef _LGN_MEM_FUNCTION
LPVOID __cdecl memcpy(LPVOID pDest, LPCVOID pSrc, size_t nSize)
{
	for (size_t i=0; i<nSize; i++)
		*(((LPBYTE)pDest)+i) = *(((LPBYTE)pSrc)+i);
	return pDest;
}

LPVOID __cdecl memmove(LPVOID pDest, LPCVOID pSrc, size_t nSize)
{
	if (pDest < pSrc)
	{
		for (size_t i=0; i<nSize; i++)
			*(((LPBYTE)pDest)+i) = *(((LPBYTE)pSrc)+i);
	}
	else
	{
		for (size_t i=nSize; i>0; i--)
			*(((LPBYTE)pDest)+i-1) = *(((LPBYTE)pSrc)+i-1);
	}
	return pDest;
}

int __cdecl memcmp(LPCVOID pBuff1, LPCVOID pBuff2, size_t nSize)
{
	LPBYTE pbBuff1 = (LPBYTE)pBuff1;
	LPBYTE pbBuff2 = (LPBYTE)pBuff2;
	for (size_t i=0; i<nSize; i++)
	{
		if ((*pbBuff1++) > (*pbBuff2++))
			return 1;
		if ((*pbBuff1) < (*pbBuff2))
			return -1;
	}
	return 0;
}

LPVOID __cdecl memset(LPVOID pDest, int nFill, size_t nSize)
{
	for (size_t i=0; i<nSize; i++)
		*(((LPBYTE)pDest)+i) = (BYTE)nFill;	
	return pDest;
}
#endif // #ifdef _LGN_MEM_FUNCTION

int __cdecl atexit(void (__cdecl *pf)(void))
{
	int nRet = 0;	

	::EnterCriticalSection(&LGN::g_csInit);

#ifdef _LGN_SUPPORT_C_EXCEPT
	__try
#endif	
	{
		if(__onexitbegin == NULL)
		{
			__onexitbegin = (_PVFV*)::HeapAlloc( LGN::g_hProcessHeap, 0, 16*sizeof(_PVFV) );
			if(__onexitbegin == NULL)
			{
				::LeaveCriticalSection(&LGN::g_csInit);
				return(-1);
			}
			__onexitend = __onexitbegin;
		}

		size_t nCurrentSize = ::HeapSize( LGN::g_hProcessHeap, 0, __onexitbegin );
		if((nCurrentSize+sizeof(_PVFV)) < ULONG(((const BYTE*)__onexitend-(const BYTE*)__onexitbegin)))
		{
			_PVFV* pNew;

			pNew = (_PVFV*)::HeapReAlloc( LGN::g_hProcessHeap, 0, __onexitbegin, 2*nCurrentSize );
			if(pNew == NULL)
			{
				::LeaveCriticalSection(&LGN::g_csInit);	  
				return(-1);
			}
		}

		*__onexitend = pf;
		__onexitend++;
	}
#ifdef _LGN_SUPPORT_C_EXCEPT
	__except (1)
	{
		nRet = -1;
	}
#endif	

	::LeaveCriticalSection(&LGN::g_csInit);	

	return(nRet);
}

extern "C" int __cdecl _purecall(void)    
{    
    return 0;    
} 

// Declare DllMain
#ifdef _USRDLL
BOOL WINAPI DllMain(HINSTANCE hDllHandle, UINT32 dwReason, LPVOID lpReserved);
BOOL WINAPI _DllMainCRTStartup(HINSTANCE hDllHandle, UINT32 dwReason, LPVOID lpReserved)
{
	BOOL bResult = TRUE;

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		LGN::DoInitialization();
	}

	bResult = DllMain(hDllHandle, dwReason, lpReserved);

	if (dwReason == DLL_PROCESS_DETACH)
	{
		LGN::DoCleanup();
	}

	return bResult;
}
#else
// Declare _tWinMain
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
extern "C" void _tWinMainCRTStartup()
{
	LGN::DoInitialization();

	STARTUPINFO StartupInfo;
	StartupInfo.dwFlags = 0;
	::GetStartupInfo(&StartupInfo);

	LPTSTR lpszCommandLine = ::GetCommandLine();
	int nRet = _tWinMain(::GetModuleHandle(NULL), NULL, lpszCommandLine,
		(StartupInfo.dwFlags & STARTF_USESHOWWINDOW) ?
		StartupInfo.wShowWindow : SW_SHOWDEFAULT);

	LGN::DoCleanup();

	::ExitProcess((UINT)nRet);
}
#endif

#endif
