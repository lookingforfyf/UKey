#ifndef _LGNUNIX_WINTEST_H__
#define _LGNUNIX_WINTEST_H__

#pragma comment(lib, "Ws2_32.lib")


#define LGN_BASETYPE_DECLARE
#define LGN_SYSTEMTIME_DECLARE

class CWinTestInit
{
public:
	CWinTestInit()
	{
		WSADATA wsaData;
		::WSAStartup(MAKEWORD(1,0), &wsaData);
	}
};

__declspec(selectany) CWinTestInit g_winTestInit;

inline void close(int h)
{
}

inline void usleep(unsigned int microseconds)
{
	::Sleep(microseconds / 1000);
}


inline int readlink(char *pszSysFile, char * pszPath, int nSize)
{
	return ::GetModuleFileNameA(NULL, pszPath, nSize);
}

inline char * dirname(char * pszPath)
{
	::PathRemoveFileSpecA(pszPath);
	return pszPath;
}

#define _DLFCN_H
#define RTLD_NOW 0

inline void *dlopen(const char *filename, int flag)
{
	return ::LoadLibraryA(filename);
}

inline void *dlsym(void *handle, const char *symbol)
{
	return ::GetProcAddress((HMODULE)handle, symbol);
}

inline int dlclose(void *handle)
{
	return ::FreeLibrary((HMODULE)handle);
}


#endif
