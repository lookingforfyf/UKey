/*! \file lgnwin.file.h 
*  \brief WIN系统的文件操作集类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  声明文件操作类CFile，临时文件操作模板类CTemporaryFileT，文件映射操作模板类CFileMappingT
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNWIN_FILE_H__
#define __LGNWIN_FILE_H__


#ifndef __LGN_BASE_H__
#error lgnwin.file.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 文件操作类
	/*! 调用系统API实现文件打开及读写功能；来源atlfile.h，雷同<B>ATL::CAtlFile</B>，具体参考MSDN 。 
		-# 点击这里查看 \ref file_example_page
	*/
	class CFile : public CHandle
	{
	public:
		//! 提供表示流中的参考点以供进行查找的字段
		enum SeekOrigin {
			Begin = FILE_BEGIN, //!< 指定流的开头
			Current = FILE_CURRENT, //!< 指定流内的当前位置
			End = FILE_END //!< 指定流的结尾
		};

		//! 提供表示文件控制权限
		enum DesiredAccess {
			accessRead =		GENERIC_READ, //!< 读权限
			accessWrite =		GENERIC_WRITE, //!< 写权限
			accessReadWrite =	GENERIC_READ | GENERIC_WRITE, //!< 读写权限
		};

		//! 提供表示文件共享模式
		enum ShareMode {
			shareRead =			FILE_SHARE_READ, //!< 共享读模式
			shareWrite =		FILE_SHARE_WRITE, //!< 共享写模式
			shareReadWrite =    FILE_SHARE_READ | FILE_SHARE_WRITE, //!< 共享读写模式
		};

		//! 提供表示文件创建方式
		enum CreationDisposition {
			createAlways =		CREATE_ALWAYS, //!< 总是创建文件 
			openExisting =		OPEN_EXISTING, //!< 打开已经存在的文件 
			openAlways =		OPEN_ALWAYS, //!< 打开文件，如果不存在则创建
		};

		//! 提供表示文件属性
		enum FileAttributes {
			attributeNormal =		FILE_ATTRIBUTE_NORMAL,
		};

	public:
		//! 构造函数
		CFile()
		{
		}
		//! 构造函数，外部传入文件对象
		CFile( CFile & file ) : CHandle( file )  // Transfers ownership
		{
		}
		//! 构造函数，外部传入文件句柄
		explicit CFile( HANDLE hFile ) : CHandle( hFile )  // Takes ownership
		{
		}

		/*! \brief 打开/创建文件
			\param[in] pszFileName ANSI编码的文件名
			\param[in] dwDesiredAccess 访问文件的控制权限DesiredAccess
			\param[in] dwShareMode 文件共享模式ShareMode
			\param[in] dwCreationDisposition 文件创建方式CreationDisposition
			\param[in] dwFlagsAndAttributes = attributeNormal 标志和文件属性FileAttributes
			\param[in] lpsa = NULL 安全描述符
			\param[in] hTemplateFile = NULL 一个模板文件句柄
			\return HRESULT
		*/
		HRESULT Create(LPCSTR pszFileName, UINT32 dwDesiredAccess, UINT32 dwShareMode, UINT32 dwCreationDisposition, UINT32 dwFlagsAndAttributes = CFile::attributeNormal, LPSECURITY_ATTRIBUTES lpsa = NULL, HANDLE hTemplateFile = NULL)
		{
			HANDLE hFile = ::CreateFileA(pszFileName, dwDesiredAccess, dwShareMode, lpsa, dwCreationDisposition,	dwFlagsAndAttributes,hTemplateFile);
			if (hFile == INVALID_HANDLE_VALUE)
				return API::HResultFromError( ::GetLastError() );

			CHandle::Attach(hFile);
			return S_OK;
		}
		/*! \brief 打开/创建文件
			\param[in] pszFileName UNICODE编码的文件名
			\param[in] dwDesiredAccess 访问文件的控制权限#DesiredAccess
			\param[in] dwShareMode 文件共享模式#ShareMode
			\param[in] dwCreationDisposition 文件创建方式#CreationDisposition
			\param[in] dwFlagsAndAttributes = attributeNormal 标志和文件属性#FileAttributes
			\param[in] lpsa = NULL 安全描述符
			\param[in] hTemplateFile = NULL 一个模板文件句柄
			\return HRESULT
		*/
		HRESULT Create(LPCWSTR pszFileName, UINT32 dwDesiredAccess, UINT32 dwShareMode, UINT32 dwCreationDisposition, UINT32 dwFlagsAndAttributes = CFile::attributeNormal, LPSECURITY_ATTRIBUTES lpsa = NULL, HANDLE hTemplateFile = NULL)
		{
			HANDLE hFile = ::CreateFileW(pszFileName, dwDesiredAccess, dwShareMode, lpsa, dwCreationDisposition,	dwFlagsAndAttributes,hTemplateFile);
			if (hFile == INVALID_HANDLE_VALUE)
				return API::HResultFromError( ::GetLastError() );

			CHandle::Attach(hFile);
			return S_OK;
		}

		/*! \brief 从文件读取指定长度的数据
			\param[out] pBuffer 缓冲区（存放读到的数据）
			\param[in] nBufSize 指定长度（缓冲区长度）
			\return HRESULT
		*/
		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize)
		{
			DWORD nBytesRead = 0;
			if (!::ReadFile(m_h, pBuffer, nBufSize, &nBytesRead, NULL) )
				return API::HResultFromError( ::GetLastError() );
			if (nBytesRead != nBufSize)
				return API::HResultFromError( ERROR_HANDLE_EOF );

			return S_OK;
		}

		/*! \brief 从文件读取数据
			\param[out] pBuffer 缓冲区（存放读到的数据）
			\param[in] nBufSize 缓冲区长度
			\param[out] nBytesRead 读到的长度
			\return HRESULT
		*/
		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize,UINT32& nBytesRead)
		{
			if (!::ReadFile(m_h, pBuffer, nBufSize, (LPDWORD)&nBytesRead, NULL))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 异步方式从文件读取数据			
			\param[out] pBuffer 缓冲区（存放读到的数据）
			\param[in] nBufSize 缓冲区长度
			\param[in] pOverlapped 异步（重叠）的结构指针。
			\return HRESULT
			\note 这个函数通常会返回API::HResultFromError（ERROR_IO_PENDING），调用GetOverlappedResult完成操作；要使用异步读写，创建文件必须设置FILE_FLAG_OVERLAPPED标志。
		*/
		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize, LPOVERLAPPED pOverlapped)
		{
			if (!::ReadFile(m_h, pBuffer, nBufSize, NULL, pOverlapped))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 完成例程方式从文件读取数据			
			\param[out] pBuffer 缓冲区（存放读到的数据）
			\param[in] nBufSize 缓冲区长度
			\param[in] pOverlapped 异步（重叠）的结构指针。
			\param[in] pfnCompletionRoutine 完成例程的结构指针。
			\return HRESULT
		*/		
		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
		{
			if (!::ReadFileEx(m_h, pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 往文件写入数据
			\param[out] pBuffer 缓冲区（存放待写的数据）
			\param[in] nBufSize 待写入数据长度
			\param[out] pnBytesWritten = NULL 写成功的长度
			\return HRESULT
		*/
		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPUINT32 pnBytesWritten = NULL)
		{
			DWORD nBytesWritten = 0;
			if (pnBytesWritten == NULL)
				pnBytesWritten = (LPUINT32)&nBytesWritten;
			if (!::WriteFile(m_h, pBuffer, nBufSize, (LPDWORD)pnBytesWritten, NULL))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 异步方式往文件写入数据	
			\param[in] pBuffer 缓冲区（存放待写的数据）
			\param[in] nBufSize 缓冲区长度
			\param[in] pOverlapped 异步（重叠）的结构指针。
			\return HRESULT
			\note 这个函数通常会返回API::HResultFromError（ERROR_IO_PENDING），调用GetOverlappedResult完成操作；要使用异步读写，创建文件必须设置FILE_FLAG_OVERLAPPED标志。
		*/
		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPOVERLAPPED pOverlapped)
		{
			if (!::WriteFile(m_h, pBuffer, nBufSize, NULL, pOverlapped))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 完成例程方式往文件写入数据	
			\param[in] pBuffer 缓冲区（存放待写的数据）
			\param[in] nBufSize 缓冲区长度
			\param[in] pOverlapped 异步（重叠）的结构指针。
			\param[in] pfnCompletionRoutine 完成例程的结构指针。
			\return HRESULT
		*/
		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPOVERLAPPED pOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE pfnCompletionRoutine)
		{
			if (!::WriteFileEx(m_h, pBuffer, nBufSize, pOverlapped, pfnCompletionRoutine))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 检索指定的文件一个重叠操作的结果
			\param[in] pOverlapped 异步（重叠）的结构指针。
			\param[out] dwBytesTransferred 接收该实际上是读或写操作传输的字节数
			\param[in] bWait 如果此参数为true，函数不返回，直到操作已完成。如果这个参数为false，操作仍未完成，该函数返回API::HResultFromError(ERROR_IO_INCOMPLETE)
			\return HRESULT
		*/
		HRESULT GetOverlappedResult(LPOVERLAPPED pOverlapped, UINT32& dwBytesTransferred, bool bWait)
		{
			if (!::GetOverlappedResult(m_h, pOverlapped, (LPDWORD)&dwBytesTransferred, bWait))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 设置文件流中的位置
			\param[in] nOffset 偏移量
			\param[in] dwFrom 文件流中的参考点#SeekOrigin
			\return HRESULT
		*/
		HRESULT Seek(INT64 nOffset, UINT32 dwFrom = CFile::Current)
		{
			LGNASSERT(dwFrom == CFile::Begin || dwFrom == CFile::End || dwFrom == CFile::Current);

			LARGE_INTEGER liOffset;
			liOffset.QuadPart = nOffset;
			DWORD nNewPos = ::SetFilePointer(m_h, liOffset.LowPart, &liOffset.HighPart, dwFrom);
			if (nNewPos == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr = API::HResultFromError( ::GetLastError() );
				if (FAILED(hr))
					return hr;
			}

			return S_OK;
		}

		/*! \brief 获取文件流中的位置
			\param[out] nPos 文件流中的位置
			\return HRESULT
		*/
		HRESULT GetPosition(UINT64& nPos) const
		{
			LARGE_INTEGER liOffset;
			liOffset.QuadPart = 0;
			liOffset.LowPart = ::SetFilePointer(m_h, 0, &liOffset.HighPart, CFile::Current);
			if (liOffset.LowPart == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr = API::HResultFromError( ::GetLastError() );
				if (FAILED(hr))
					return hr;
			}
			nPos = liOffset.QuadPart;

			return S_OK;
		}

		/*! \brief 把缓存数据刷到文件实体上
			\return HRESULT
		*/
		HRESULT Flush()
		{
			if (!::FlushFileBuffers(m_h))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		
		/*! \brief 锁定由调用进程独占访问指定的文件。
			\param[in] nPos 要锁定文件位置的起始字节偏移量
			\param[in] nCount 要锁定的字节长度
			\return RESULT
		*/
		HRESULT LockRange(UINT64 nPos, UINT64 nCount)
		{
			LARGE_INTEGER liPos;
			liPos.QuadPart = nPos;

			LARGE_INTEGER liCount;
			liCount.QuadPart = nCount;

			if (!::LockFile(m_h, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 解锁由调用进程独占访问指定的文件。
			\param[in] nPos 要解锁文件位置的起始字节偏移量
			\param[in] nCount 要解锁的字节长度
			\return RESULT
		*/
		HRESULT UnlockRange(UINT64 nPos, UINT64 nCount)
		{
			LARGE_INTEGER liPos;
			liPos.QuadPart = nPos;

			LARGE_INTEGER liCount;
			liCount.QuadPart = nCount;

			if (!::UnlockFile(m_h, liPos.LowPart, liPos.HighPart, liCount.LowPart, liCount.HighPart))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 设置文件大小
			\param[in] nNewLen 文件大小
			\return RESULT
		*/
		HRESULT SetSize(UINT64 nNewLen)
		{
			HRESULT hr = Seek(nNewLen, CFile::Begin);
			if (FAILED(hr))
				return hr;

			if (!::SetEndOfFile(m_h))
				return API::HResultFromError( ::GetLastError() );

			return S_OK;
		}

		/*! \brief 获取文件大小
			\param[out] nLen 文件大小
			\return RESULT
		*/
		HRESULT GetSize(UINT64& nLen) const
		{
			ULARGE_INTEGER liFileSize;
			liFileSize.LowPart = ::GetFileSize(m_h, &liFileSize.HighPart);
			if (liFileSize.LowPart == INVALID_FILE_SIZE)
			{
				HRESULT hr = API::HResultFromError( ::GetLastError() );
				if (FAILED(hr))
					return hr;
			}

			nLen = liFileSize.QuadPart;

			return S_OK;
		}
	};

/*! \page file_example_page CFile例子
<pre>	%CFile类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnfile.h"

	// ...
	static bool GetAllData(LPCTSTR pszFileName, LGN::CByteBuffer & buff)
	{
		bool bRet = false;
		LGN::CFile file;
		if (SUCCEEDED(file.Create(pszFileName, LGN::CFile::accessRead, LGN::CFile::shareRead, LGN::CFile::openExisting)))
		{
			UINT64 nFileSize = 0;
			file.GetSize(nFileSize);
			bRet = SUCCEEDED(file.Read(buff.GetBufferSetLength((int)nFileSize), (int)nFileSize));
			file.Close();
		}
		return bRet;
	}
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 临时文件操作模板静态类
	template< typename _BaseType = CHAR>
	class TemporaryFileTraits
	{
	public:
		static HRESULT GenerateFileName(LPCSTR pszDir, LPSTR pszTempFileName)
		{
			CHAR szPath[MAX_PATH]; 
			CHAR tmpFileName[MAX_PATH]; 

			if (pszDir == NULL)
			{
				DWORD dwRet = ::GetTempPathA(MAX_PATH, szPath);
				if (dwRet == 0)
				{
					// Couldn't find temporary path;
					return API::HResultFromError(::GetLastError());
				}
				else if (dwRet > MAX_PATH)
				{
					return DISP_E_BUFFERTOOSMALL;
				}
			}
			else
			{
				::lstrcpynA(szPath, pszDir, MAX_PATH);
			}

			if (!::GetTempFileNameA(szPath, "TFR", 0, tmpFileName))
			{
				// Couldn't create temporary filename;
				return API::HResultFromError(::GetLastError());
			}
			tmpFileName[MAX_PATH-1]='\0';

			::lstrcpynA(pszTempFileName, tmpFileName, MAX_PATH);
			return S_OK;
		}

		static HRESULT DeleteOrMoveFile(LPCSTR szNewName, LPCSTR pszTempFileName)
		{
			// no new name so delete it
			if (szNewName == NULL)
			{
				::DeleteFileA(pszTempFileName);
				return S_OK;
			}

			// delete any existing file and move our temp file into it's place
			if (!::DeleteFileA(szNewName))
			{
				DWORD dwError = ::GetLastError();
				if (dwError != ERROR_FILE_NOT_FOUND)
					return API::HResultFromError(dwError);
			}

			if (!::MoveFileA(pszTempFileName, szNewName))
				return API::HResultFromError(::GetLastError());

			return S_OK;
		}
	};

	//! 临时文件操作模板静态类
	template< >
	class TemporaryFileTraits< WCHAR >
	{
	public:
		static HRESULT GenerateFileName(LPCWSTR pszDir, LPWSTR pszTempFileName)
		{
			WCHAR szPath[MAX_PATH]; 
			WCHAR tmpFileName[MAX_PATH]; 

			if (pszDir == NULL)
			{
				DWORD dwRet = ::GetTempPathW(MAX_PATH, szPath);
				if (dwRet == 0)
				{
					// Couldn't find temporary path;
					return API::HResultFromError(::GetLastError());
				}
				else if (dwRet > MAX_PATH)
				{
					return DISP_E_BUFFERTOOSMALL;
				}
			}
			else
			{
				::lstrcpynW(szPath, pszDir, MAX_PATH);
			}

			if (!::GetTempFileNameW(szPath, L"TFR", 0, tmpFileName))
			{
				// Couldn't create temporary filename;
				return API::HResultFromError(::GetLastError());
			}
			tmpFileName[MAX_PATH-1]='\0';

			::lstrcpynW(pszTempFileName, tmpFileName, MAX_PATH);
			return S_OK;
		}

		static HRESULT DeleteOrMoveFile(LPCWSTR szNewName, LPCWSTR pszTempFileName)
		{
			// no new name so delete it
			if (szNewName == NULL)
			{
				::DeleteFileW(pszTempFileName);
				return S_OK;
			}

			// delete any existing file and move our temp file into it's place
			if (!::DeleteFileW(szNewName))
			{
				DWORD dwError = ::GetLastError();
				if (dwError != ERROR_FILE_NOT_FOUND)
					return API::HResultFromError(dwError);
			}

			if (!::MoveFileW(pszTempFileName, szNewName))
				return API::HResultFromError(::GetLastError());

			return S_OK;
		}
	};

	//! 临时文件操作模板类
	/*! 调用CFile实现临时文件创建及读写功能；来源atlfile.h，雷同<B>ATL::CAtlTemporaryFile</B>，具体参考MSDN 。 
		-# 点击这里查看 \ref temporaryfile_example_page
	*/
	// This class allows the creation of a temporary file that is written to.
	// When the entire file has been successfully written it will be closed and given
	// it's proper file name if required.
	template< typename BaseType >
	class CTemporaryFileT
	{
	public:
		CTemporaryFileT()
		{
		}

		~CTemporaryFileT()
		{
			// Ensure that the temporary file is closed and deleted,
			// if necessary.
			if (m_file.m_h != NULL)
			{
				Close();
			}
		}
		
		HRESULT Create(const BaseType * pszDir = NULL, UINT32 dwDesiredAccess = GENERIC_WRITE)
		{
			HRESULT hr = TemporaryFileTraits< BaseType >::GenerateFileName(pszDir, m_szTempFileName);
			if (FAILED(hr))
				return hr;

			m_dwAccess = dwDesiredAccess;

			SECURITY_ATTRIBUTES secatt;
			secatt.nLength = sizeof(secatt);
			secatt.lpSecurityDescriptor = NULL;
			secatt.bInheritHandle = TRUE;

			return m_file.Create(
				m_szTempFileName,
				m_dwAccess,
				0,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NOT_CONTENT_INDEXED | FILE_ATTRIBUTE_TEMPORARY,
				&secatt);
		}

		HRESULT Close(const BaseType * szNewName = NULL)
		{
			// This routine is called when we are finished writing to the 
			// temporary file, so we now just want to close it and copy
			// it to the actual filename we want it to be called.

			// So let's close it first.
			m_file.Close();
			
			return TemporaryFileTraits< BaseType >::DeleteOrMoveFile(szNewName, m_szTempFileName);
		}

		HRESULT HandsOff()
		{
			m_file.Flush();
			m_file.Close();

			return S_OK;
		}

		HRESULT HandsOn()
		{
			HRESULT hr = m_file.Create(m_szTempFileName, m_dwAccess, 0, OPEN_EXISTING);
			if (FAILED(hr))
				return hr;

			return m_file.Seek(0, FILE_END);
		}

		HRESULT Read(LPVOID pBuffer,UINT32 nBufSize, UINT32& nBytesRead)
		{
			return m_file.Read(pBuffer, nBufSize, nBytesRead);
		}

		HRESULT Write(LPCVOID pBuffer,UINT32 nBufSize, LPUINT32 pnBytesWritten = NULL)
		{
			return m_file.Write(pBuffer, nBufSize, pnBytesWritten);
		}

		HRESULT Seek(INT64 nOffset, UINT32 dwFrom = FILE_CURRENT)
		{
			return m_file.Seek(nOffset, dwFrom);
		}

		HRESULT GetPosition(UINT64& nPos)
		{
			return m_file.GetPosition(nPos);
		}

		HRESULT Flush()
		{
			return m_file.Flush();
		}

		HRESULT LockRange(UINT64 nPos, UINT64 nCount)
		{
			return m_file.LockRange(nPos, nCount);
		}

		HRESULT UnlockRange(UINT64 nPos, UINT64 nCount)
		{
			return m_file.UnlockRange(nPos, nCount);
		}

		HRESULT SetSize(UINT64 nNewLen)
		{
			return m_file.SetSize(nNewLen);
		}

		HRESULT GetSize(UINT64& nLen)
		{
			return m_file.GetSize(nLen);
		}

		operator HANDLE()
		{
			return m_file;
		}

		const BaseType * TempFileName()
		{
			return m_szTempFileName;
		}

	private:
		CFile m_file;
		BaseType m_szTempFileName[MAX_PATH];
		DWORD m_dwAccess;
	};

	typedef CTemporaryFileT< CHAR > CTemporaryFileA;
	typedef CTemporaryFileT< WCHAR > CTemporaryFileW;
	typedef CTemporaryFileT< TCHAR > CTemporaryFile;


	
/*! \page temporaryfile_example_page CTemporaryFileT例子
<pre>	%CTemporaryFileT类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnfile.h"

	// ...
	LGN::CTemporaryFile tmpFile;

	tmpFile.Create();
	tmpFile.Write("123", 3)
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 共享内存文件操作模板静态类
	template< typename _BaseType = CHAR>
	class FileMappingTraits
	{
	public:
		static HANDLE SCreateFileMapping(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, UINT32 flProtect, UINT32 dwMaximumSizeHigh, UINT32 dwMaximumSizeLow, LPCSTR lpName)
		{
			return ::CreateFileMappingA(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}
		static HANDLE SOpenFileMapping(UINT32 dwDesiredAccess, bool bInheritHandle, LPCSTR lpNameOpenFileMapping)
		{
			return ::OpenFileMappingA(dwDesiredAccess, bInheritHandle, lpNameOpenFileMapping);
		}
	};

	//! 共享内存文件操作模板静态类
	template< >
	class FileMappingTraits< WCHAR >
	{
	public:
		static HANDLE SCreateFileMapping(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, UINT32 flProtect, UINT32 dwMaximumSizeHigh, UINT32 dwMaximumSizeLow, LPCWSTR lpName)
		{
			return ::CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
		}
		static HANDLE SOpenFileMapping(UINT32 dwDesiredAccess, bool bInheritHandle, LPCWSTR lpNameOpenFileMapping)
		{
			return ::OpenFileMappingW(dwDesiredAccess, bInheritHandle, lpNameOpenFileMapping);
		}
	};

	//! 共享内存文件操作模板类
	/*! 调用系统API实现共享内存文件创建及读写功能；来源atlfile.h，雷同<B>ATL::CAtlFileMappingBase</B>，具体参考MSDN 。 
		-# 点击这里查看 \ref filemapping_example_page
	*/
	template< typename BaseType >
	class CFileMappingT
	{
	public:
		CFileMappingT()
		{
			m_pData = NULL;
			m_hMapping = NULL;
		}

		~CFileMappingT()
		{
			Unmap();
		}

		HRESULT MapFile(HANDLE hFile, UINT64 nMappingSize = 0,UINT64 nOffset = 0, UINT32 dwMappingProtection = PAGE_READONLY, UINT32 dwViewDesiredAccess = FILE_MAP_READ)
		{
			LGNASSERT(hFile != INVALID_HANDLE_VALUE && hFile != NULL);

			ULARGE_INTEGER liFileSize;
			liFileSize.LowPart = ::GetFileSize(hFile, &liFileSize.HighPart);
			if (liFileSize.QuadPart < nMappingSize)
				liFileSize.QuadPart = nMappingSize;

			m_hMapping = FileMappingTraits< BaseType >::SCreateFileMapping(hFile, NULL, dwMappingProtection, liFileSize.HighPart, liFileSize.LowPart, 0);
			if (m_hMapping == NULL)
				return API::HResultFromError(::GetLastError());

			if (nMappingSize == 0)
				m_nMappingSize = (liFileSize.QuadPart - nOffset);
			else
				m_nMappingSize = nMappingSize;

			m_dwViewDesiredAccess = dwViewDesiredAccess;
			m_nOffset.QuadPart = nOffset;

			m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
			if (m_pData == NULL)
			{
				HRESULT hr = API::HResultFromError(::GetLastError());
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
				return hr;
			}

			return S_OK;
		}

		HRESULT MapSharedMem(const BaseType * lpszName, UINT64 nMappingSize, bool* pbAlreadyExisted = NULL, LPSECURITY_ATTRIBUTES lpsa = NULL, UINT32 dwMappingProtection = PAGE_READWRITE, UINT32 dwViewDesiredAccess = FILE_MAP_ALL_ACCESS)
		{
			LGNASSERT(nMappingSize > 0);
			LGNASSERT(lpszName != NULL); // if you just want a regular chunk of memory, use a heap allocator

			m_nMappingSize = nMappingSize;

			ULARGE_INTEGER nSize;
			nSize.QuadPart = nMappingSize;
			m_hMapping = FileMappingTraits< BaseType >::SCreateFileMapping(INVALID_HANDLE_VALUE, lpsa, dwMappingProtection, nSize.HighPart, nSize.LowPart, lpszName);
			if (m_hMapping == NULL)
				return API::HResultFromError(::GetLastError());

			if (pbAlreadyExisted != NULL)
				*pbAlreadyExisted = (::GetLastError() == ERROR_ALREADY_EXISTS);

			m_dwViewDesiredAccess = dwViewDesiredAccess;
			m_nOffset.QuadPart = 0;

			m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, (SIZE_T)m_nMappingSize, NULL);
			if (m_pData == NULL)
			{
				HRESULT hr = API::HResultFromError(::GetLastError());
				::CloseHandle(m_hMapping);
				return hr;
			}


			return S_OK;
		}

		HRESULT OpenMapping(const BaseType * lpszName, UINT64 nMappingSize, UINT64 nOffset = 0, UINT32 dwViewDesiredAccess = FILE_MAP_ALL_ACCESS)
		{
			LGNASSERT(lpszName != NULL); // if you just want a regular chunk of memory, use a heap allocator

			m_nMappingSize = nMappingSize;
			m_dwViewDesiredAccess = dwViewDesiredAccess;

			m_hMapping = FileMappingTraits< BaseType >::SOpenFileMapping(m_dwViewDesiredAccess, FALSE, lpszName);
			if (m_hMapping == NULL)
				return API::HResultFromError(::GetLastError());

			m_dwViewDesiredAccess = dwViewDesiredAccess;
			m_nOffset.QuadPart = nOffset;

			m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, (SIZE_T)m_nMappingSize, NULL);
			if (m_pData == NULL)
			{
				HRESULT hr = API::HResultFromError(::GetLastError());
				::CloseHandle(m_hMapping);
				return hr;
			}


			return S_OK;
		}

		HRESULT Unmap()
		{
			HRESULT hr = S_OK;

			if (m_pData != NULL)
			{
				if (!::UnmapViewOfFile(m_pData))
					hr = API::HResultFromError(::GetLastError());
				m_pData = NULL;
			}
			if (m_hMapping != NULL)
			{
				if (!::CloseHandle(m_hMapping) && SUCCEEDED(hr))
					hr = API::HResultFromError(::GetLastError());
				m_hMapping = NULL;
			}
			return hr;
		}

		void* GetData() const
		{
			return m_pData;
		}

		HANDLE GetHandle() const
		{
			return m_hMapping;
		}

		UINT64 GetMappingSize()
		{
			return m_nMappingSize;
		}

		HRESULT CopyFrom( CFileMappingT& orig)
		{
			if (this == &orig)
				return S_OK;
			LGNASSERT(orig.m_pData != NULL);
			LGNASSERT(orig.m_hMapping != NULL);

			m_dwViewDesiredAccess = orig.m_dwViewDesiredAccess;
			m_nOffset.QuadPart = orig.m_nOffset.QuadPart;
			m_nMappingSize = orig.m_nMappingSize;

			if (!::DuplicateHandle(::GetCurrentProcess(), orig.m_hMapping, GetCurrentProcess(),
				&m_hMapping, NULL, TRUE, DUPLICATE_SAME_ACCESS))
				return API::HResultFromError(::GetLastError());

			m_pData = ::MapViewOfFileEx(m_hMapping, m_dwViewDesiredAccess, m_nOffset.HighPart, m_nOffset.LowPart, m_nMappingSize, NULL);
			if (m_pData == NULL)
			{
				HRESULT hr = API::HResultFromError(::GetLastError());
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
				return hr;
			}

			return S_OK;
		}

		CFileMappingT(CFileMappingT& orig) throw()
		{
			m_pData = NULL;
			m_hMapping = NULL;

			HRESULT hr = CopyFrom(orig);
			if (FAILED(hr))
				LgnThrow(hr);
		}

		CFileMappingT& operator=(CFileMappingT& orig) throw()
		{
			HRESULT hr = CopyFrom(orig);
			if (FAILED(hr))
				LgnThrow(hr);

			return *this;
		}

	private:
		void* m_pData;
		UINT64 m_nMappingSize;
		HANDLE m_hMapping;
		ULARGE_INTEGER m_nOffset;
		DWORD m_dwViewDesiredAccess;
	};

	typedef CFileMappingT< CHAR > CFileMappingA;
	typedef CFileMappingT< WCHAR > CFileMappingW;
	typedef CFileMappingT< TCHAR > CFileMapping;

/*! \page filemapping_example_page CFileMappingT例子
<pre>	%CFileMappingT类的使用例子</pre>
<pre>
\#include "lgnbase.h"
\#include "lgnfile.h"

	// ...
	LGN::CFileMapping fileMap, fileMap2;

	// app1
	if (fileMap.MapSharedMem("mysharedmemory", 1024) == S_OK)
	{
		LGN::API::memcpy(fileMap.GetData(), "123", 3);
	}

	// app2
	fileMap2.OpenMapping("mysharedmemory", 1024);
	void * ptr = fileMap2.GetData();
</pre>
*/
	
}; // namespace LGN

#endif // __LGNWIN_FILE_H__
