#ifndef __LGNUNIX_SYNC_H__
#define __LGNUNIX_SYNC_H__

#ifndef IPC_WAIT
#define IPC_WAIT 0
#endif

#define LGN_SYNC_DEF_ACCESS 0666  //-八进制
#define LGN_SYNC_INFINITE	0xFFFFFFFF  // Infinite timeout


namespace LGN
{
	// Sync Classes


	//! 同步机制操作模板静态类
	template< typename _BaseType = CHAR>
	class SyncTraits
	{
	public:
		static int SCreateEvent(LPVOID lpEventAttributes, bool bManualReset, bool bInitialState, LPCSTR lpName)
		{
			return SCreateSemaphore(lpEventAttributes, bInitialState?1:0, 1, lpName);
		}
		static int SOpenEvent(UINT32 unDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			return SOpenSemaphore(unDesiredAccess, bInheritHandle, lpName);
		}

		static int SCreateMutex(LPVOID lpMutexAttributes, bool bInitialOwner, LPCSTR lpName)
		{
			return SCreateSemaphore(lpMutexAttributes, 1, 1, lpName);
		}
		static int SOpenMutex(UINT32 unDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			return SOpenSemaphore(unDesiredAccess, bInheritHandle, lpName);
		}

		static int SCreateSemaphore(LPVOID lpSemaphoreAttributes, INT32 lInitialCount, INT32 lMaximumCount, LPCSTR lpName)
		{
			key_t semkey = 0x17800000 + CalcCRC16(lpName);
			union semun
			{
				int              val;    /* Value for SETVAL */
				struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
				unsigned short  *array;  /* Array for GETALL, SETALL */
				struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
			}arg;

			int semid = semget(semkey, lMaximumCount, LGN_SYNC_DEF_ACCESS|IPC_CREAT|IPC_EXCL|SEM_UNDO);  
			if (semid == -1)
			{
				if (errno == EEXIST)
				{			
					//printf("semget EEXIST(%d)\n", EEXIST);							
					// flags只包含了IPC_CREAT标志, 参数nsems(这里为1)必须与原来的信号灯数目一致
					semid = semget(semkey, lMaximumCount, LGN_SYNC_DEF_ACCESS|IPC_CREAT|SEM_UNDO);

					bool init_ok = false;
					struct semid_ds sem_info;
					arg.buf = &sem_info;					
					for(int i=0; i<3; i++)
					{
						if(semctl(semid, 0, IPC_STAT, arg)==-1)
						{
							//perror("semctl error");
							break;
						}
						else
						{
							if(arg.buf->sem_otime != 0)
							{
								init_ok = true;
								break;
							}
							sleep(1); 
						}
					}

					// do some initializing, here we assume that the first process that creates the sem will
					// finish initialize the sem and run semop in max_tries*1 seconds. else it will not run
					// semop any more.
					if(!init_ok)
					{
						//printf("set val = %d\n", lInitialCount);
						arg.val = lInitialCount;
						if(semctl(semid,0,SETVAL,arg) == -1)
						{
							LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("semctl 0x%x\n"), errno);
						}
					}
				}
				else // if (errno == EEXIST)
				{
					LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("semget 0x%x\n"), errno);
					//return API::HResultFromError( errno );					
				}
			}
			else
			{
				//printf("set val = %d\n", lInitialCount);
				/*设置信号量的初始值，就是资源个数*/  
				arg.val = lInitialCount;
				if (semctl(semid, 0, SETVAL, arg) < 0)
				{
					LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("semctl 0x%x\n"), errno);
				}
			}
			return semid;
		}
		static int SOpenSemaphore(UINT32 unDesiredAccess, bool bInheritHandle, LPCSTR lpName)
		{
			key_t semkey = 0x17800000 + CalcCRC16(lpName);

			int semid = semget(semkey, 0, LGN_SYNC_DEF_ACCESS);
			if(semid == -1)
			{
				LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("semget 0x%x\n"), errno);
			}
			return semid;
		}

	public:
		static HRESULT SetSemaphoreValue(int semid, int op, int flg)
		{
			struct sembuf sem_p;  
			sem_p.sem_num = 0;  
			sem_p.sem_op = op;  
			sem_p.sem_flg = flg | SEM_UNDO;
			if(semop(semid,&sem_p,1) < 0)
			{
				int errno_t = errno;
				LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("semop 0x%x\n"), errno_t);
				return API::HResultFromError( errno_t );	
			}
			int i = semctl(semid, 0, GETVAL, 0);
			LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Error, TEXT("GETVAL semid:0x%x i:%d op %d\n"), semid,i,op);

			return S_OK;
		}


		static int CalcCRC16(LPCSTR s)
		{
			unsigned short crc=0, s_s, bit;
			int len = strlen(s);
			for(int i=0; i<len; i++)
			{
				s_s = (unsigned)s[i];
				for(int j=0; j<8; j++)
				{
					bit = crc&1;
					if (s_s&1)
						bit ^= 1;
					if (bit)
						crc ^= 0x4002;
					crc >>= 1;
					if (bit)
						crc|=0x8000;
					s_s >>= 1;
				}
			}
			return crc;
		}
	};


	class CCriticalSection
	{
	public:
#if (LGN_SUPPORT_MULTITHREAD)
		CCriticalSection() throw()
		{
			int rv = pthread_mutex_init(&m_mutex, NULL);
			if (rv != 0)
				LgnThrow(API::HResultFromError(rv));
		}
		~CCriticalSection()
		{
			pthread_mutex_destroy(&m_mutex);
		}

		void Enter() throw() 
		{
			int rv = pthread_mutex_lock(&m_mutex);
			if (rv != 0)
				LgnThrow(API::HResultFromError(rv));
		}
		void Leave() throw()
		{
			int rv = pthread_mutex_unlock(&m_mutex);
			if (rv != 0)
				LgnThrow(API::HResultFromError(rv));
		}

		bool TryEnter()
		{
			return( pthread_mutex_trylock(&m_mutex) == 0 );
		}

	protected:
		pthread_mutex_t m_mutex;
#else
		void Enter() {}			
		void Leave() {}			
#endif
	};


	template< typename BaseType >
	class CEventT : public CHandle
	{
	public:
		CEventT() throw()
		{
		}
		CEventT( CEventT& hEvent ) : CHandle( hEvent )
		{
			m_semid = hEvent.m_semid;
		}
		CEventT( bool bManualReset, bool bInitialState )  throw()
		{
			HRESULT hr = Create(NULL, bManualReset, bInitialState, NULL);
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		CEventT( LPVOID pSecurity, bool bManualReset, bool bInitialState, const BaseType * pszName )  throw()
		{
			HRESULT hr = Create(pSecurity, bManualReset, bInitialState, pszName);
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		explicit CEventT( HANDLE h ) : CHandle( h )
		{
			m_semid = (h == NULL)? -1 : *((int *)h);
		}

		//! Create a new event
		HRESULT Create( LPVOID pSecurity, bool bManualReset, bool bInitialState, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			m_semid = SyncTraits< BaseType >::SCreateEvent(pSecurity, bManualReset, bInitialState, pszName);
			if (m_semid == -1)
			{
				return E_FAIL;
			}
			m_h = &m_semid;
			return S_OK;
		}
		//! Open an existing named event
		HRESULT Open( UINT32 dwAccess, bool bInheritHandle, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			m_semid = SyncTraits< BaseType >::SOpenEvent(dwAccess, bInheritHandle, pszName);
			if (m_semid == -1)
			{
				return E_FAIL;
			}
			m_h = &m_semid;
			return S_OK;
		}
		//! Pulse the event (signals waiting objects, then resets)
		HRESULT Pulse()
		{
			LGNASSERT( m_h != NULL );
			// 不支持
			return S_OK;
		}
		//! Set the event to the non-signaled state
		HRESULT Reset()
		{
			LGNASSERT( m_h != NULL );
			// 不支持
			return S_OK;
		}
		//! Set the event to the signaled state
		HRESULT Set()
		{
			LGNASSERT( m_h != NULL );

			return SyncTraits< BaseType >::SetSemaphoreValue(m_semid, 1, IPC_WAIT); // SEM_UNDO- 进程退出时自动还原所做操作
		}

	public:
		HRESULT WaitFor(UINT32 unMilliseconds = LGN_SYNC_INFINITE)
		{
			LGNASSERT( m_h != NULL );

			return SyncTraits< BaseType >::SetSemaphoreValue(m_semid, -1, IPC_WAIT);
		}

		HRESULT Destroy()
		{
			//Close semaphore
			if (semctl(m_semid,1,IPC_RMID,0) < 0)
			{
				return API::HResultFromError( errno );	
			}
			return S_OK;
		}

	protected:		
		int m_semid;
	};

	typedef CEventT< CHAR > CEventA;
	//typedef CEventT< WCHAR > CEventW;
	typedef CEventT< TCHAR > CEvent;


	template< typename BaseType = char >
	class CMutexT : public CHandle
	{
	public:
		CMutexT() : m_semid(-1)
		{
		}
		CMutexT( CMutexT& hMutex ) : CHandle( hMutex )
		{
			m_semid = hMutex.m_semid;
		}
		explicit CMutexT( bool bInitialOwner ) throw()
		{
			HRESULT hr = Create( NULL, bInitialOwner, NULL );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		CMutexT( LPVOID pSecurity, bool bInitialOwner, const BaseType * pszName ) throw()
		{
			HRESULT hr = Create( pSecurity, bInitialOwner, pszName );
			if( FAILED(hr) )
			{
				LgnThrow(hr);
			}
		}
		explicit CMutexT( HANDLE h ) : CHandle( h )
		{
			m_semid = (h == NULL)? -1 : *((int *)h);
		}

		HRESULT Create( LPVOID pSecurity, bool bInitialOwner, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			m_semid = SyncTraits< BaseType >::SCreateMutex(pSecurity, bInitialOwner, pszName);
			if (m_semid == -1)
			{
				return E_FAIL;
			}
			m_h = &m_semid;
			return S_OK;
		}

		HRESULT Open( UINT32 dwAccess, bool bInheritHandle, const BaseType * pszName )
		{
			LGNASSERT( m_h == NULL );

			m_semid = SyncTraits< BaseType >::SOpenMutex(dwAccess, bInheritHandle, pszName);
			if (m_semid == -1)
			{
				return E_FAIL;
			}
			m_h = &m_semid;
			return S_OK;
		}

		HRESULT Release()
		{
			LGNASSERT( m_h != NULL );

			return SyncTraits< BaseType >::SetSemaphoreValue(m_semid, 1, IPC_WAIT);
		}

	public:
		HRESULT WaitFor(UINT32 unMilliseconds = LGN_SYNC_INFINITE)
		{
			LGNASSERT( m_h != NULL );

			return SyncTraits< BaseType >::SetSemaphoreValue(m_semid, -1, IPC_WAIT);
		}

		HRESULT Destroy()
		{
			//Close semaphore
			if (semctl(m_semid,1,IPC_RMID,0) < 0)
			{
				return API::HResultFromError( errno );	
			}
			return S_OK;
		}

	protected:		
		int m_semid;
	};

	typedef CMutexT< CHAR > CMutexA;
	//typedef CMutexT< WCHAR > CMutexW;
	typedef CMutexT< TCHAR > CMutex;


	class CMutexLock
	{
	public:
		CMutexLock( HANDLE hMutex, bool bInitialLock = true, UINT32 dwMilliseconds = INFINITE ) : m_hMutex(hMutex), m_bLocked(false)
		{
			if( bInitialLock )
			{
				Lock(dwMilliseconds);
			}
		}
		CMutexLock()
		{
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

			if (m_hMutex == NULL)
			{
				LgnThrow(E_LGN_INVALID_DATA);
			}

			SyncTraits< CHAR >::SetSemaphoreValue(*((int*)m_hMutex), -1, IPC_WAIT);
			m_bLocked = true;
		}
		void Unlock()
		{
			LGNASSERT( m_bLocked );

			if (m_hMutex == NULL)
			{
				LgnThrow(E_LGN_INVALID_DATA);
			}

			SyncTraits< CHAR >::SetSemaphoreValue(*((int*)m_hMutex), 1, IPC_WAIT);
			m_bLocked = false;
		}
		void SetHandle(HANDLE hMutex)
		{
			m_hMutex = hMutex;
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

#endif // __LGNUNIX_SYNC_H__



/*
// 将 Windows IPC 应用程序移植到 Linux，第 2 部分: 信号量和事件
//		http://blog.csdn.net/zhuky/archive/2009/12/16/5017254.aspx
//*/
