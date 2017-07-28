/*! \file lgnwin.thread.h
*  \brief WIN系统的线程类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  声明线程类CThread
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_THREAD_H__
#define __LGNWIN_THREAD_H__


#ifndef __LGN_BASE_H__
#error lgnwin.thread.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 线程类
	/*! 调用系统API实现线程创建及相关操作，该类为虚拟类，必须继承后才可使用 
		-# 点击这里查看 \ref thread_example_page
	*/
	class CThread : public CHandle
	{
	public:
		//! 构造函数
		CThread()
		{
		}
		//! 构造函数，外部传入线程对象
		CThread( CThread & th ) : CHandle( th )
		{
		}
		//! 构造函数，外部传入线程句柄
		explicit CThread( HANDLE hThread ) : CHandle( hThread )
		{
		}

		/*! \brief 创建一个线程并运行
			\param[in] nPriority 线程优先级
			\return HRESULT
		*/
		HRESULT Start(int nPriority = THREAD_PRIORITY_NORMAL)
		{
			if (m_h != NULL)
			{
				DWORD dwExitCode = 0;
				if (::GetExitCodeThread(m_h, &dwExitCode) && (dwExitCode == STILL_ACTIVE))
					return API::HResultFromError( ERROR_SERVICE_EXISTS );
			}

			DWORD dwThreadId;
#if !defined(_ATL_MIN_CRT) && defined(_MT) && defined(_INC_PROCESS)
			HANDLE hThread = (HANDLE) _beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *)) _ThreadProc, this, CREATE_SUSPENDED, (unsigned int *)&dwThreadId);
#else
			HANDLE hThread = ::CreateThread(NULL, 0, _ThreadProc, this, CREATE_SUSPENDED, &dwThreadId);
#endif
			if (hThread == NULL)
			{
				return API::HResultFromError( ::GetLastError() );
			}

			if (nPriority != THREAD_PRIORITY_NORMAL)
			{
				::SetThreadPriority(hThread, nPriority);
			}

			::ResumeThread(hThread);

			CHandle::Attach(hThread);
			return S_OK;
		}

		/*! \brief 终止线程运行
			\param[in] unExitCode 线程退出的代号
			\return HRESULT
		*/
		HRESULT Terminate(UINT32 unExitCode = 0)
		{
			if (!::TerminateThread(m_h, unExitCode))
			{
				return API::HResultFromError( ::GetLastError() );
			}

			return S_OK;
		}

		/*! \brief 等待线程退出
			\param[in] unTimeout 等待的超时时间
			\return HRESULT
		*/
		HRESULT Wait(UINT32 unTimeout = INFINITE)
		{
			HRESULT hr = S_OK;
			switch (::WaitForSingleObject(m_h, unTimeout))
			{
			case WAIT_OBJECT_0:
				break;
			case WAIT_FAILED:
				hr = API::HResultFromError( ::GetLastError() );
				break;
			case WAIT_ABANDONED:
			case WAIT_TIMEOUT:
			default:
				hr = API::HResultFromError( WAIT_TIMEOUT );
				break;
			}
			return hr;
		}

	protected:
		/*! \brief 线程执行函数
			\return 线程退出的代号
		*/
		virtual UINT32 OnRun() = 0;

		/*! \brief 静态的线程执行函数
			\param[in] lpThreadParameter 线程参数
			\return 线程退出的代号
		*/
		static DWORD WINAPI _ThreadProc(LPVOID lpThreadParameter)
		{
			return ((CThread*)lpThreadParameter)->OnRun();
		}
	};

/*! \page thread_example_page CThread例子
<pre>	%CThread类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnthread.h"

class CMyThread : public LGN::CThread
{
protected:
	UINT32 OnRun()
	{
		for (int i=0; i<100; i++)
		{
			printf("%d\n", i);
			LGN::API::Sleep(20);
		}
		return 0;
	}
};
	// ...
	CMyThread my1;
	my1.Start();
	my1.Wait();
</pre>
*/

	
}; // namespace LGN

#endif // __LGNWIN_THREAD_H__
