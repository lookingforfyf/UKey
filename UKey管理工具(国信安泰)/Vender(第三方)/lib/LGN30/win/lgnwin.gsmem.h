/*! \file lgnwin.gsmem.h 
*  \brief WIN系统的全局共享内存操作类
*
*  工程要包含本文件，依赖文件：lgnbase.h, lgnwin.ver.h, lgnwin.auth.h, lgnwin.file.h
*
*  声明全局共享内存操作模板静态类GlobalSharedMemoryTraits，全局互斥体操作模板类CGlobalMutexT，全局共享内存操作模板类CGlobalSharedMemoryT
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_GSMEM_H__
#define __LGNWIN_GSMEM_H__


#ifndef __LGN_BASE_H__
#error lgnwin.gsmem.h requires include lgnbase.h
#endif

#ifndef __LGNWIN_VER_H__
#error lgnwin.gsmem.h requires include lgnwin.ver.h
#endif

#ifndef __LGNWIN_AUTH_H__
#error lgnwin.gsmem.h requires include lgnwin.auth.h
#endif

#ifndef __LGNWIN_FILE_H__
#error lgnwin.gsmem.h requires include lgnwin.file.h
#endif

#ifndef __LGNWIN_SYNC_H__
#error lgnwin.gsmem.h requires include lgnwin.sync.h
#endif

#include <WtsApi32.h>
#pragma comment(lib, "Wtsapi32.lib")


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//! 全局共享内存操作模板静态类
	template< typename _BaseType = CHAR>
	class GlobalSharedMemoryTraits
	{
	public:
		//! 生成全局的共享内存文件名称，格式：Global\\\<pszName\>
		static void GenerateFileName(LPCSTR pszName, LPSTR pszGlibalFileName)
		{
			LGN::ChTraitsExA::FormatNP(pszGlibalFileName, MAX_PATH, "Global\\%s", pszName);
		}
		//! 生成会话及的共享内存文件名称，格式：Session\\\<unSessionId\>\<pszName\>
		static void GenerateFileName(UINT32 unSessionId, LPCSTR pszName, LPSTR pszGlibalFileName)
		{
			LGN::ChTraitsExA::FormatNP(pszGlibalFileName, MAX_PATH, "Session\\%d\\%s", unSessionId, pszName);
		}
	};

	//! 全局共享内存操作模板静态类
	template< >
	class GlobalSharedMemoryTraits< WCHAR >
	{
	public:
		//! 生成全局的共享内存文件名称，格式：Global\\\<pszName\>
		static void GenerateFileName(LPCWSTR pszName, LPWSTR pszGlibalFileName)
		{
			LGN::ChTraitsExW::FormatNP(pszGlibalFileName, MAX_PATH, L"Global\\%s", pszName);
		}
		//! 生成会话及的共享内存文件名称，格式：Session\\\<unSessionId\>\<pszName\>
		static void GenerateFileName(UINT32 unSessionId, LPCWSTR pszName, LPWSTR pszGlibalFileName)
		{
			LGN::ChTraitsExW::FormatNP(pszGlibalFileName, MAX_PATH, L"Session\\%d\\%s", unSessionId, pszName);
		}
	};


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 全局互斥体操作模板类
	/*! 继承于CMutexT，主要配合全局共享内存操作模板类CGlobalSharedMemoryT一起使用，保护其安全读写。
	*/
	template< typename BaseType >
	class CGlobalMutexT : public CMutexT< BaseType >
	{
	public:
		//! 构造函数
		CGlobalMutexT()
		{
		}
		//! 构造函数，外部传入全局互斥体对象
		CGlobalMutexT( CGlobalMutexT& hMutex ) : CHandle( hMutex )
		{
		}
		//! 构造函数，外部传入全局互斥体句柄
		explicit CGlobalMutexT( HANDLE h ) : CHandle( h )
		{
		}

		/*! \brief 创建全局互斥体
			\param[in] lpszName 互斥体名称
			\param[in] bInitialOwner 如果这个值是true，然后调用者创建互斥体，调用线程获得互斥对象的初始所有权。否则，调用线程不会获得互斥的所有权。如果要判断对方造成的互斥，查看返回值部分。
			\param[in] lpsa 内核对象的安全属性
			\return 错误号
			\note 其函数的参数顺序与基类CMutexT::Create不同
		*/
		HRESULT Create( const BaseType * lpszName, bool bInitialOwner, LPSECURITY_ATTRIBUTES lpsa)
		{
			BaseType szMutexName[MAX_PATH];
			GlobalSharedMemoryTraits< BaseType >::GenerateFileName(lpszName, szMutexName);

			return CMutexT< BaseType >::Create(lpsa, bInitialOwner, szMutexName);
		}
		//! 以安全属性g_saLowIntegrity来创建全局互斥体
		HRESULT Create( const BaseType * lpszName, bool bInitialOwner = false )
		{
			return Create(lpszName, bInitialOwner, &g_saLowIntegrity);
		}

		/*! \brief 打开已存在的全局互斥体
			\param[in] lpszName 互斥体名称
			\param[in] bInheritHandle = false 如果这个值是true，进程继承该句柄。否则，该进程不继承这个句柄。
			\param[in] dwAccess 内核对象的安全属性
			\return 错误号
			\note 其函数的参数顺序与基类CMutexT::Open不同
		*/
		HRESULT Open( const BaseType * lpszName, bool bInheritHandle = false, UINT32 dwAccess = MUTEX_ALL_ACCESS )
		{
			BaseType szMutexName[MAX_PATH];
			GlobalSharedMemoryTraits< BaseType >::GenerateFileName(lpszName, szMutexName);

			return CMutexT< BaseType >::Open(dwAccess, bInitialOwner, szMutexName);
		}
	};

	typedef CGlobalMutexT< CHAR > CGlobalMutexA;
	typedef CGlobalMutexT< WCHAR > CGlobalMutexW;
	typedef CGlobalMutexT< TCHAR > CGlobalMutex;



////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 全局共享内存操作模板类
	/*! 内部定义个CFileMappingT对象，根据操作系统不同的版本来创建，vista以后版本只能用会话级的共享内存（全局共享内存只能在会话0上创建）
		-# 点击这里查看 \ref globalsharedmemory_example_page
	*/
	template< typename BaseType >
	class CGlobalSharedMemoryT
	{
	public:
		//! 构造函数，初始成员变量
		CGlobalSharedMemoryT() : m_bAlreadyExisted(false), m_dwOwnerSessionId(0), m_dwCreatorSessionId(0)
		{
		}
		//! 虚造函数
		~CGlobalSharedMemoryT()
		{
		}

		/*! \brief 创建全局共享内存
			\param[in] lpszName 共享内存文件名称
			\param[in] dwFileSize 要创建的共享内存大小
			\param[in] lpsa 内核对象的安全属性
			\return 错误号
			\note 在vista以后系统，不同会话下调用本函数，会创建多个共享内存；尽可能调用OpenAlways来创建共享内存，防止出现该问题
		*/
		HRESULT Create(const BaseType * lpszName, UINT32 dwFileSize, PSECURITY_ATTRIBUTES lpsa)
		{
			BaseType szFileName[MAX_PATH];
			if (g_osVersionInfo.IsVistaLater())
			{
				if (m_dwOwnerSessionId == 0)
					::ProcessIdToSessionId(::GetCurrentProcessId(), &m_dwOwnerSessionId);

				m_dwCreatorSessionId = m_dwOwnerSessionId;
				GlobalSharedMemoryTraits< BaseType >::GenerateFileName(m_dwCreatorSessionId, lpszName, szFileName);
			}
			else
			{
				GlobalSharedMemoryTraits< BaseType >::GenerateFileName(lpszName, szFileName);
			}

			return m_fileMapping.MapSharedMem(szFileName, dwFileSize, &m_bAlreadyExisted, lpsa);
		}
		//! 以安全属性g_saLowIntegrity来创建全局共享内存
		HRESULT Create(const BaseType * lpszName, UINT32 dwFileSize)
		{
			return Create(lpszName, dwFileSize, &g_saLowIntegrity);
		}

		/*! \brief 打开全局共享内存
			\param[in] lpszName 共享内存文件名称
			\param[in] dwFileSize 要创建的共享内存大小
			\return 错误号
			\note 在vista以后系统，尝试打开所有会话下对应的共享内存
		*/
		HRESULT Open(const BaseType * lpszName, UINT32 dwFileSize)
		{
			HRESULT hr = S_OK;
			BaseType szFileName[MAX_PATH];
			if (g_osVersionInfo.IsVistaLater())
			{
				if (m_dwOwnerSessionId == 0)
					::ProcessIdToSessionId(::GetCurrentProcessId(), &m_dwOwnerSessionId);

				PWTS_SESSION_INFO pSessionInfo = NULL;
				DWORD dwSessionCount = 0;
				::WTSEnumerateSessions( WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pSessionInfo, &dwSessionCount);
				for (DWORD i=0; i<dwSessionCount; i++)
				{
					GlobalSharedMemoryTraits< BaseType >::GenerateFileName(pSessionInfo[i].SessionId, lpszName, szFileName);
					if (SUCCEEDED( hr = m_fileMapping.OpenMapping(szFileName, dwFileSize) ))
					{
						m_dwCreatorSessionId = pSessionInfo[i].SessionId;
						break;
					}
				}
				::WTSFreeMemory(pSessionInfo);
			}
			else
			{
				GlobalSharedMemoryTraits< BaseType >::GenerateFileName(lpszName, szFileName);
				hr = m_fileMapping.OpenMapping(szFileName, dwFileSize);
			}

			m_bAlreadyExisted = SUCCEEDED(hr);
			return hr;
		}

		/*! \brief 总是打开全局共享内存
			\param[in] lpszName 共享内存文件名称
			\param[in] dwFileSize 要创建的共享内存大小
			\param[in] lpsa 内核对象的安全属性
			\return 错误号
			\see 如果本对象的共享内存句柄有效则关闭，先调用Open，如果失败则再调用Create
		*/
		HRESULT OpenAlways(const BaseType * lpszName, UINT32 dwFileSize, PSECURITY_ATTRIBUTES lpsa)
		{
			if (m_fileMapping.GetHandle() != NULL)
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
		//! 以安全属性g_saLowIntegrity来总是打开全局共享内存
		HRESULT OpenAlways(const BaseType * lpszName, UINT32 dwFileSize)
		{
			return OpenAlways(lpszName, dwFileSize, &g_saLowIntegrity);
		}

		/*! \brief 关闭已打开的全局共享对象句柄
			\return 错误号
		*/
		HRESULT Close()
		{
			m_bAlreadyExisted = false;
			m_dwOwnerSessionId = 0;
			m_dwCreatorSessionId = 0;
			return m_fileMapping.Unmap();
		}

		//! 检测当前全局共享是否有效
		bool IsValid() const
		{
			if (m_fileMapping.GetHandle() == NULL)
				return false;

			if (m_dwOwnerSessionId != m_dwCreatorSessionId) // windows vista later
			{
				PWTS_SESSION_INFO pSessionInfo = NULL;
				DWORD dwSessionCount = 0;
				::WTSEnumerateSessions( WTS_CURRENT_SERVER_HANDLE, NULL, 1, &pSessionInfo, &dwSessionCount);

				bool bRet = false;
				for (DWORD i=0; i<dwSessionCount; i++)
				{
					if (pSessionInfo[i].SessionId == m_dwCreatorSessionId) // 创建会话还存在
					{				
						bRet = true;
						break;
					}
				}
				::WTSFreeMemory(pSessionInfo);
				return bRet;
			}
			return true;
		}

		//! 在调用Create前，全局共享是否已存在		
		bool IsAlreadyExisted() const
		{
			return m_bAlreadyExisted;
		}

		//! 返回当前进程所属的会话Id（vista以上）
		UINT32 GetOwnerSessionId() const
		{
			return m_dwOwnerSessionId;
		}
		//! 返回创建共享内存的会话Id（vista以上）
		UINT32 GetCreatorSessionId() const
		{
			return m_dwCreatorSessionId;
		}

		//! 返回内部共享内存文件映射对象的引用
		CFileMappingT< BaseType > & GetFileMapping()
		{
			return m_fileMapping;
		}

		//! 返回内部共享内存的存储位置
		void* GetData() const
		{
			return m_fileMapping.GetData();
		}

	protected:
		CFileMappingT< BaseType > m_fileMapping; //!< 内部共享内存文件映射对象
		bool m_bAlreadyExisted; //!< 表示在调用Create前，全局共享是否已存在
		DWORD m_dwOwnerSessionId; //!< 当前进程所属的会话Id
		DWORD m_dwCreatorSessionId; //!< 创建共享内存的会话Id
	};


	typedef CGlobalSharedMemoryT< CHAR > CGlobalSharedMemoryA;
	typedef CGlobalSharedMemoryT< WCHAR > CGlobalSharedMemoryW;
	typedef CGlobalSharedMemoryT< TCHAR > CGlobalSharedMemory;

/*! \page globalsharedmemory_example_page CGlobalSharedMemoryT例子
<pre>	%CGlobalSharedMemoryT类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgngsmem.h"

	// ...
	LGN::CGlobalSharedMemoryA smemA;
	LGN::CGlobalSharedMemoryW smemW;

	if (smemA.OpenAlways("mysharedmem", 1024) == S_OK)
		LGN::API::memcpy(smemA.GetData(), "123", 3);

	smemW.OpenAlways(L"mysharedmem", 1024, &LGN::g_saLowIntegrity);
	void * ptr = smemW.GetData();
</pre>
*/

}; // namespace LGN

#endif // __LGNWIN_GSMEM_H__
