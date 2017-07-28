/*! \file lgnwin.sync.h 
*  \brief WIN系统的同步机制集类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  声明线程临界区保护类CCriticalSection，事件操作模板类CEventT，互斥体操作模板类CMutexT，信号量操作模板类CSemaphoreT<br>
*  及互斥体锁定操作类CMutexLock
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_SYNC_H__
#define __LGNWIN_SYNC_H__

#ifndef __LGN_BASE_H__
#error lgnwin.gsmem.h requires include lgnbase.h
#endif

#define LGN_SYNC_DEF_ACCESS MUTEX_ALL_ACCESS
#define LGN_SYNC_INFINITE	INFINITE


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// Sync Classes


	//! 同步机制操作模板静态类
	template< typename _BaseType = CHAR>
	class SyncTraits
	{
	public:
		static HANDLE SCreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, bool bManualReset, bool bInitialState, LPCSTR lpName)
		{
			return ::CreateEventA( lpEventAttributes, bManualReset, bInitialState, lpName );
		}
		static HANDLE SOpenEvent(UINT32 dwDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			return ::OpenEventA( dwDesiredAccess, bInheritHandle, lpName );
		}

		static HANDLE SCreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, bool bInitialOwner, LPCSTR lpName)
		{
			return ::CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
		}
		static HANDLE SOpenMutex(UINT32 dwDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			return ::OpenMutexA(dwDesiredAccess, bInheritHandle, lpName);
		}

		static HANDLE SCreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, INT32 lInitialCount, INT32 lMaximumCount, LPCSTR lpName)
		{
			return ::CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
		}
		static HANDLE SOpenSemaphore(UINT32 dwDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			return ::OpenSemaphoreA(dwDesiredAccess, bInheritHandle, lpName);
		}
	};

	//! 同步机制操作模板静态类
	template< >
	class SyncTraits< WCHAR >
	{
	public:
		static HANDLE SCreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes, bool bManualReset, bool bInitialState, LPCWSTR lpName)
		{
			return ::CreateEventW( lpEventAttributes, bManualReset, bInitialState, lpName );
		}
		static HANDLE SOpenEvent(UINT32 dwDesiredAccess, bool bInheritHandle, LPCWSTR lpName)
		{
			return ::OpenEventW( dwDesiredAccess, bInheritHandle, lpName );
		}

		static HANDLE SCreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, bool bInitialOwner, LPCWSTR lpName)
		{
			return ::CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
		}
		static HANDLE SOpenMutex(UINT32 dwDesiredAccess, bool bInheritHandle, LPCWSTR lpName)
		{
			return ::OpenMutexW(dwDesiredAccess, bInheritHandle, lpName);
		}

		static HANDLE SCreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, INT32 lInitialCount, INT32 lMaximumCount, LPCWSTR lpName)
		{
			return ::CreateSemaphoreW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
		}
		static HANDLE SOpenSemaphore(UINT32 dwDesiredAccess, bool bInheritHandle, LPCWSTR lpName)
		{
			return ::OpenSemaphoreW(dwDesiredAccess, bInheritHandle, lpName);
		}
	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 线程临界区保护类
	/*! 来源 atlsync.h，同<B>ATL::CCriticalSection</B>，具体参考MSDN
	*/
	class CCriticalSection : public CRITICAL_SECTION
	{
	public:
		CCriticalSection() throw()
		{
			_LGN__TRY
			{
				::InitializeCriticalSection( this );
			}
			_LGN__EXCEPT( STATUS_NO_MEMORY == ::GetExceptionCode() )
			{
				LgnThrow( E_OUTOFMEMORY );
			}
		}

#if (_WIN32_WINNT >= 0x0403)
		CCriticalSection( UINT nSpinCount )
		{
			_LGN__TRY
			{
				BOOL bRet = ::InitializeCriticalSectionAndSpinCount( this, (ULONG)nSpinCount );
				if (!bRet)
				{
					LgnThrow( E_OUTOFMEMORY );
				}
			} 
			_LGN__EXCEPT(STATUS_NO_MEMORY == GetExceptionCode()) 
			{
				LgnThrow( E_OUTOFMEMORY );
			}
		}
#endif

		~CCriticalSection()
		{
			::DeleteCriticalSection( this );
		}

		// Acquire the critical section
		void Enter()
		{
			::EnterCriticalSection( this );				
		}

		// Release the critical section
		void Leave()
		{
			::LeaveCriticalSection( this );
		}

#if (_WIN32_WINNT >= 0x0403)
		// Set the spin count for the critical section
		UINT SetSpinCount( UINT nSpinCount )
		{
			return( (UINT)::SetCriticalSectionSpinCount( this, (ULONG)nSpinCount ) );
		}
#endif
#if (_WIN32_WINNT >= 0x0400)
		// Attempt to acquire the critical section
		 bool TryEnter()
		{
			return( ::TryEnterCriticalSection( this ) == TRUE);
		}
#endif
	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 事件操作模板类
	/*! 来源 atlsync.h，雷同<B>ATL::CEvent</B>，返回值使用错误号，具体参考MSDN
	*/
	template< typename BaseType >
	class CEventT : public CHandle
	{
	public:
		CEventT() throw()
		{
		}
		CEventT( CEventT& hEvent ) : CHandle( hEvent )
		{
		}
		CEventT( bool bManualReset, bool bInitialState )  throw()
		{
			HRESULT hr = Create(NULL, bManualReset, bInitialState, NULL);
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		CEventT( LPSECURITY_ATTRIBUTES pSecurity, bool bManualReset, bool bInitialState, const BaseType * pszName )  throw()
		{
			HRESULT hr = Create(pSecurity, bManualReset, bInitialState, pszName);
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		explicit CEventT( HANDLE h ) : CHandle( h )
		{
		}

		//! Create a new event
		HRESULT Create( LPSECURITY_ATTRIBUTES pSecurity, bool bManualReset, bool bInitialState, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SCreateEvent(pSecurity, bManualReset, bInitialState, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return  S_OK;
		}
		//! Open an existing named event
		HRESULT Open( UINT32 dwAccess, bool bInheritHandle, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SOpenEvent(dwAccess, bInheritHandle, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return  S_OK;
		}
		//! Pulse the event (signals waiting objects, then resets)
		HRESULT Pulse()
		{
			LGNASSERT( m_h != NULL );

			if (!::PulseEvent( m_h ))
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Set the event to the non-signaled state
		HRESULT Reset()
		{
			LGNASSERT( m_h != NULL );

			if (!::ResetEvent( m_h ))
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Set the event to the signaled state
		HRESULT Set()
		{
			LGNASSERT( m_h != NULL );

			if (!::SetEvent( m_h ))
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}

	public:
		//! wait for event
		HRESULT WaitFor(UINT32 unMilliseconds = LGN_SYNC_INFINITE)
		{
			LGNASSERT( m_h != NULL );

			return ::WaitForSingleObject(m_h, unMilliseconds);
		}

		//! nothing
		HRESULT Destroy()
		{
			return S_OK;
		}
	};

	typedef CEventT< CHAR > CEventA;
	typedef CEventT< WCHAR > CEventW;
	typedef CEventT< TCHAR > CEvent;



////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 互斥体操作模板类
	/*! 来源 atlsync.h，雷同<B>ATL::CMutex</B>，返回值使用错误号，具体参考MSDN
	*/
	template< typename BaseType >
	class CMutexT : public CHandle
	{
	public:
		CMutexT()
		{
		}
		CMutexT( CMutexT& hMutex ) : CHandle( hMutex )
		{
		}
		explicit CMutexT( bool bInitialOwner ) throw()
		{
			HRESULT hr = Create( NULL, bInitialOwner, NULL );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		CMutexT( LPSECURITY_ATTRIBUTES pSecurity, bool bInitialOwner, const BaseType * pszName ) throw()
		{
			HRESULT hr = Create( pSecurity, bInitialOwner, pszName );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		explicit CMutexT( HANDLE h ) : CHandle( h )
		{
		}

		//! Create a new mutex
		HRESULT Create( LPSECURITY_ATTRIBUTES pSecurity, bool bInitialOwner, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SCreateMutex(pSecurity, bInitialOwner, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Open an existing named mutex
		HRESULT Open( UINT32 dwAccess, bool bInheritHandle, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SOpenMutex(dwAccess, bInheritHandle, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Release ownership of the mutex
		HRESULT Release()
		{
			LGNASSERT( m_h != NULL );

			if (!::ReleaseMutex( m_h ))
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}

	public:
		//! wait for event
		HRESULT WaitFor(UINT32 unMilliseconds = LGN_SYNC_INFINITE)
		{
			LGNASSERT( m_h != NULL );

			return ::WaitForSingleObject(m_h, unMilliseconds);
		}

		//! nothing
		HRESULT Destroy()
		{
			return S_OK;
		}
	};

	typedef CMutexT< CHAR > CMutexA;
	typedef CMutexT< WCHAR > CMutexW;
	typedef CMutexT< TCHAR > CMutex;


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 信号量操作模板类
	/*! 来源 atlsync.h，雷同<B>ATL::CSemaphore</B>，返回值使用错误号，具体参考MSDN
	*/
	template< typename BaseType >
	class CSemaphoreT : public CHandle
	{
	public:
		CSemaphoreT()
		{
		}
		CSemaphoreT( CSemaphoreT& hSemaphore ) : CHandle( hSemaphore )
		{
		}
		CSemaphoreT( INT32 nInitialCount, INT32 nMaxCount ) throw()
		{
			HRESULT hr = Create( NULL, nInitialCount, nMaxCount, NULL );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		CSemaphoreT( LPSECURITY_ATTRIBUTES pSecurity, INT32 nInitialCount, INT32 nMaxCount, const BaseType * pszName ) throw()
		{
			HRESULT hr = Create( pSecurity, nInitialCount, nMaxCount, pszName );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		explicit CSemaphoreT( HANDLE h ) throw() : CHandle(h)
		{
		}

		//! Create a new semaphore
		HRESULT Create( LPSECURITY_ATTRIBUTES pSecurity, INT32 nInitialCount, INT32 nMaxCount, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SCreateSemaphore(pSecurity, nInitialCount, nMaxCount, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Open an existing named semaphore
		HRESULT Open( UINT32 dwAccess, bool bInheritHandle, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			if ((m_h = SyncTraits< BaseType >::SOpenSemaphore(dwAccess, bInheritHandle, pszName)) == NULL)
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}
		//! Increase the count of the semaphore
		HRESULT Release( INT32 nReleaseCount = 1, LPINT32 pnOldCount = NULL )
		{
			LGNASSERT( m_h != NULL );

			if (!::ReleaseSemaphore( m_h, nReleaseCount, (LPLONG)pnOldCount ))
				return API::HResultFromError( ::GetLastError() );
			return S_OK;
		}

	public:
		//! wait for event
		HRESULT WaitFor(UINT32 unMilliseconds = LGN_SYNC_INFINITE)
		{
			LGNASSERT( m_h != NULL );

			return ::WaitForSingleObject(m_h, unMilliseconds);
		}

		//! nothing
		HRESULT Destroy()
		{
			return S_OK;
		}
	};

	typedef CSemaphoreT< CHAR > CSemaphoreA;
	typedef CSemaphoreT< WCHAR > CSemaphoreW;
	typedef CSemaphoreT< TCHAR > CSemaphore;


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 互斥体锁定操作类
	/*! 来源 atlsync.h，雷同<B>ATL::CMutexLock</B>，增加了超时时间，具体参考MSDN
	*/
	class CMutexLock
	{
	public:
		CMutexLock( HANDLE hMutex, bool bInitialLock = true, UINT32 dwMilliseconds = INFINITE ) : m_hMutex(hMutex)
		{
			if( bInitialLock )
			{
				Lock(dwMilliseconds);
			}
		}
		~CMutexLock()
		{
			if( m_bLocked )
			{
				Unlock();
			}
		}

		void Lock(UINT32 dwMilliseconds = INFINITE)
		{
			LGNASSERT( !m_bLocked );

			UINT32 dwResult = ::WaitForSingleObject( m_hMutex, dwMilliseconds );
			if( dwResult == WAIT_ABANDONED )
			{
				LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Debug, TEXT("Warning: abandoned mutex 0x%x\n"), m_hMutex);
			}
			m_bLocked = true;
		}
		void Unlock()
		{
			LGNASSERT( m_bLocked );

			::ReleaseMutex( m_hMutex );
			//LGNASSERT in CMutexLock::Lock prevents calling Lock more than 1 time.
			m_bLocked = false;  
		}

	// Implementation
	private:
		HANDLE m_hMutex;
		bool m_bLocked;

	// Private to prevent accidental use
		CMutexLock( const CMutexLock& );
		CMutexLock& operator=( const CMutexLock& );
	};

}; // namespace LGN

#endif // __LGNWIN_SYNC_H__
