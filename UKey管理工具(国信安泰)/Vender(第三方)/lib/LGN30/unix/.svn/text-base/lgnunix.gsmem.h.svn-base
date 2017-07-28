#ifndef __LGNUNIX_GSMEM_H__
#define __LGNUNIX_GSMEM_H__


#ifndef __LGN_BASE_H__
#error lgnunix.gsmem.h requires include lgnbase.h
#endif

#ifndef __LGNUNIX_SYNC_H__
#error lgnunix.gsmem.h requires include lgnunix.sync.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename BaseType = char >
	class CGlobalMutexT : public CMutexT< BaseType >
	{
	public:
		CGlobalMutexT()
		{
		}

		HRESULT Create( const BaseType * lpszName, bool bInitialOwner, LPVOID lpsa)
		{
			return CMutexT< BaseType >::Create(lpsa, bInitialOwner, lpszName);
		}
		HRESULT Create( const BaseType * lpszName, bool bInitialOwner = false )
		{
			return CMutexT< BaseType >::Create(NULL, bInitialOwner, lpszName);
		}

		HRESULT Open( const BaseType * lpszName, bool bInheritHandle = false, UINT32 dwAccess = LGN_SYNC_DEF_ACCESS )
		{
			return CMutexT< BaseType >::Open(dwAccess, bInheritHandle, lpszName);
		}
	};

	typedef CGlobalMutexT< CHAR > CGlobalMutexA;
	//typedef CGlobalMutexT< WCHAR > CGlobalMutexW;
	typedef CGlobalMutexT< TCHAR > CGlobalMutex;



////////////////////////////////////////////////////////////////////////////////////////////////////

	template< typename BaseType = char >
	class CGlobalSharedMemoryT
	{
	public:
		CGlobalSharedMemoryT() : m_shmid(-1), m_bAlreadyExisted(false)
		{
		}
		~CGlobalSharedMemoryT()
		{
		}

		HRESULT Create(const BaseType * lpszName, UINT32 dwFileSize, LPVOID lpsa)
		{
			m_bAlreadyExisted = false;

			key_t shmkey = 0x17800000 + SyncTraits< BaseType >::CalcCRC16(lpszName);
			if ((m_shmid = shmget(shmkey, dwFileSize, LGN_SYNC_DEF_ACCESS|IPC_CREAT|IPC_EXCL)) <	0)
			{
				if (errno == EEXIST)
				{
					if ((m_shmid	= shmget(shmkey, dwFileSize, LGN_SYNC_DEF_ACCESS|IPC_CREAT)) <	0)
						return API::HResultFromError( errno );
					m_bAlreadyExisted = true;
				}
			}
			return S_OK;
		}
		HRESULT Create(const BaseType * lpszName, UINT32 dwFileSize)
		{
			return Create(lpszName, dwFileSize, NULL);
		}

		HRESULT Open(const BaseType * lpszName, UINT32 dwFileSize)
		{
			key_t shmkey = 0x17800000 + SyncTraits< BaseType >::CalcCRC16(lpszName);
			if ((m_shmid = shmget(shmkey, dwFileSize, LGN_SYNC_DEF_ACCESS)) <	0)
			{
				m_bAlreadyExisted = false;
				return API::HResultFromError( errno );
			}							
			m_bAlreadyExisted = true;
			return S_OK;
		}

		HRESULT OpenAlways(const BaseType * lpszName, UINT32 dwFileSize, LPVOID lpsa)
		{
			if (m_shmid != -1)
			{
				Close();
			}

			HRESULT hr = Open(lpszName, dwFileSize);
			if (FAILED(hr))
			{
				hr = Create(lpszName, dwFileSize, lpsa);
			}
			return hr;		
		}
		HRESULT OpenAlways(const BaseType * lpszName, UINT32 dwFileSize)
		{
			return OpenAlways(lpszName, dwFileSize, NULL);
		}

		HRESULT Close()
		{
			m_bAlreadyExisted = false;
			shmdt(GetData()); // shmctl(m_shmid, IPC_RMID, NULL);
			m_shmid = -1;
		}

		bool IsValid() const
		{
			return true;
		}

		bool IsAlreadyExisted() const
		{
			return m_bAlreadyExisted;
		}

		UINT32 GetOwnerSessionId() const
		{
			return 0;
		}
		UINT32 GetCreatorSessionId() const
		{
			return 0;
		}

		void* GetData() const
		{
			return shmat(m_shmid,0,0);
		}

	public:
		HRESULT Destroy()
		{
			if (shmctl(m_shmid,IPC_RMID,0) < 0)
			{
				return API::HResultFromError( errno );	
			}
			return S_OK;
		}

	protected:
		int	m_shmid;
		bool m_bAlreadyExisted;		
	};


	typedef CGlobalSharedMemoryT< CHAR > CGlobalSharedMemoryA;
	//typedef CGlobalSharedMemoryT< WCHAR > CGlobalSharedMemoryW;
	typedef CGlobalSharedMemoryT< TCHAR > CGlobalSharedMemory;

}; // namespace LGN

#endif // __LGNUNIX_GSMEM_H__
