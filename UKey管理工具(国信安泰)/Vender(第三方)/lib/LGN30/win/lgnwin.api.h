/*! \file lgnwin.api.h 
 *  \brief WIN系统的API简单封装
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNWIN_API_H__
#define __LGNWIN_API_H__

//#pragma once


/////////////////////////////////////////////////////////////////////////////
// Win32 libraries


namespace LGN
{
	namespace API //! 系统API操作命名空间
	{
		//! 转换错误号，保证其值小于0
		LGN_INLINE HRESULT HResultFromError(INT32 nError)
		{
			return HRESULT_FROM_WIN32(nError);
		}

		//! memory 在定义_LGN_NO_DEFAULT_LIBS下重定向
		LGN_INLINE void memcpy(LPVOID pDest, LPCVOID pSrc, size_t nSize)
		{
			::memcpy(pDest, pSrc, nSize);
			//::CopyMemory(pDest, pSrc, nSize);
		}
		//! memmove 在定义_LGN_NO_DEFAULT_LIBS下重定向
		LGN_INLINE void memmove(LPVOID pDest, LPCVOID pSrc, size_t nSize)
		{
			::memmove(pDest, pSrc, nSize);
			//::MoveMemory(pDest, pSrc, nSize);
		}
		//! memcpy 在定义_LGN_NO_DEFAULT_LIBS下重定向
		LGN_INLINE int memcmp(LPCVOID pBuff1, LPCVOID pBuff2, size_t nSize)
		{
			return ::memcmp(pBuff1, pBuff2, nSize);
		}		
		//! memset 在定义_LGN_NO_DEFAULT_LIBS下重定向
		LGN_INLINE void memset(LPVOID pDest, BYTE btFill, size_t nSize)
		{
			::memset(pDest, btFill, nSize);
			//::FillMemory(pDest, nSize, btFill);
		}
		//! \see memset(\a pDest, 0, \a nSize)
		LGN_INLINE void memzero(LPVOID pDest, size_t nSize)
		{
			::memset(pDest, 0, nSize);
			//::ZeroMemory(pDest, nSize);
		}

		//! 获取ANSI字符串长度，\a pszStr为NULL时返回0
		LGN_INLINE int SafeStringLength(LPCSTR pszStr)
		{
			return (pszStr == NULL)? 0 : ::lstrlenA(pszStr);
		}
		//! 获取UNICODE字符串长度，\a pszStr为NULL时返回0
		LGN_INLINE int SafeStringLength(LPCWSTR pszStr)
		{
			return (pszStr == NULL)? 0 : ::lstrlenW(pszStr);
		}

		//! 加一（线程安全）。内部调用<B>InterlockedIncrement</B>，具体参考MSDN 
		LGN_INLINE ULONG SafeIncrement(ULONG * pVal)
		{
			return ::InterlockedIncrement((PLONG)pVal);
		}
		//! 减一（线程安全）。内部调用<B>InterlockedDecrement</B>，具体参考MSDN 
		LGN_INLINE ULONG SafeDecrement(ULONG * pVal)
		{
			return ::InterlockedDecrement((PLONG)pVal);
		}
		LGN_INLINE ULONG SafeExchange(ULONG* pVal,ULONG Value)
		{
			return ::InterlockedExchange(pVal,Value);
		}

		//! 关闭内核句柄。内部调用<B>CloseHandle</B>，具体参考MSDN 
		LGN_INLINE void CloseHandle(HANDLE hObject)
		{
			::CloseHandle(hObject);
		}

		//! 当前线程睡眠等待。内部调用<B>Sleep</B>，具体参考MSDN 
		LGN_INLINE void Sleep(UINT32 dwMilliseconds)
		{
			::Sleep(dwMilliseconds);
		}

		//! 返回系统启动后的总时间。内部调用<B>GetTickCount</B>，具体参考MSDN 
		LGN_INLINE UINT32 GetTickCount()
		{
			return ::GetTickCount();
		}

		//! 获取当前区域时间。内部调用<B>GetLocalTime</B>，具体参考MSDN 
		LGN_INLINE void GetLocalTime(LPSYSTEMTIME lpSystemTime)
		{
			::GetLocalTime(lpSystemTime);
		}

		//! 获取当前进程ID。内部调用<B>GetCurrentProcessId</B>，具体参考MSDN 
		LGN_INLINE PID_T GetCurrentProcessId()
		{
			return ::GetCurrentProcessId();
		}
		//! 获取当前线程ID。内部调用<B>GetCurrentThreadId</B>，具体参考MSDN 
		LGN_INLINE TID_T GetCurrentThreadId()
		{
			return ::GetCurrentThreadId();
		}

	}; // namespace API

}; // namespace LGN

#endif
