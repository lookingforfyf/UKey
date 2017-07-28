#ifndef __LGNUNIX_FILE_H__
#define __LGNUNIX_FILE_H__


#ifndef __LGN_BASE_H__
#error lgnwin.file.h requires include lgnbase.h
#endif


namespace LGN
{
	// class CFile
	class CFile : public CHandle
	{
	public:
		enum SeekOrigin {
			Begin = SEEK_SET,
			Current = SEEK_CUR,
			End = SEEK_END
		};

		enum DesiredAccess {
			accessRead =		0x00000001,
			accessWrite =		0x00000002,
			accessReadWrite =	accessRead | accessWrite,
		};

		enum ShareMode {
			shareRead =			0x00000100,
			shareWrite =		0x00000200,
			shareReadWrite =    shareRead | shareWrite,
		};

		enum CreationDisposition {
			createAlways =		0x00010000,
			openExisting =		0x00020000,
			openAlways =		0x00040000,
		};


		CFile()
		{
		}
		CFile( CFile & file )
		{
			m_h = file.Detach();
		}
		explicit CFile( HANDLE hFile )
		{
			m_h = hFile;
		}
		~CFile()
		{
			CloseSelf();
		}
			

		HRESULT Create(LPCSTR pszFilename, UINT32 dwDesiredAccess, UINT32 dwShareMode, UINT32 dwCreationDisposition, UINT32 dwFlagsAndAttributes = 0, LPVOID lpsa = NULL, HANDLE hTemplateFile = NULL)
		{
			if ((m_h = ::fopen(pszFilename, "r+b")) == NULL)
			{
				if ((dwCreationDisposition & CFile::openExisting) == 0)
					m_h = ::fopen(pszFilename, "w+b");
			}
			return (m_h != NULL)? S_OK : E_FAIL;
		}
		HRESULT Create(LPCWSTR pszFilename, UINT32 dwDesiredAccess, UINT32 dwShareMode, UINT32 dwCreationDisposition, UINT32 dwFlagsAndAttributes = 0, LPVOID lpsa = NULL, HANDLE hTemplateFile = NULL)
		{
			LGN::LgnThrow(E_INVALIDARG);
			return E_FAIL;
		}

		HRESULT CloseSelf()
		{
			if (m_h != NULL)
			{
				::fclose((FILE *)m_h);
				m_h = NULL;
			}
			return S_OK;
		}

		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize)
		{
			size_t nLen = ::fread(pBuffer, 1, nBufSize, (FILE *)m_h);
			return (nLen == nBufSize)? S_OK : E_FAIL;
		}

		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize,UINT32& nBytesRead)
		{
			nBytesRead = (UINT32)::fread(pBuffer, 1, nBufSize, (FILE *)m_h);
			return S_OK;
		}

		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize, LPVOID pOverlapped)
		{
			return Read(pBuffer, nBufSize);
		}

		HRESULT Read(LPVOID pBuffer, UINT32 nBufSize, LPVOID pOverlapped, LPVOID pfnCompletionRoutine)
		{
			return Read(pBuffer, nBufSize);
		}

		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPUINT32 pnBytesWritten = NULL)
		{
			size_t nLen = ::fwrite(pBuffer, 1, nBufSize, (FILE *)m_h);
			if (pnBytesWritten != NULL)
				*pnBytesWritten = (UINT32)nLen;
			return (nLen == nBufSize)? S_OK : E_FAIL;
		}

		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPVOID pOverlapped)
		{
			return Write(pBuffer, nBufSize);
		}

		HRESULT Write(LPCVOID pBuffer, UINT32 nBufSize, LPVOID pOverlapped, LPVOID pfnCompletionRoutine)
		{
			return Write(pBuffer, nBufSize);
		}

		HRESULT GetOverlappedResult(LPVOID pOverlapped, UINT32& dwBytesTransferred, bool bWait)
		{
			return E_FAIL;
		}

		HRESULT Seek(INT64 nOffset, UINT32 dwFrom = CFile::Current)
		{
			LGNASSERT(dwFrom == CFile::Begin || dwFrom == CFile::End || dwFrom == CFile::Current);

			if (::fseek((FILE *)m_h, (long)nOffset, dwFrom) != 0)
				return E_FAIL;
			return ::ftell((FILE *)m_h);
		}

		HRESULT GetPosition(UINT64& nPos) const
		{
			nPos = ::ftell((FILE *)m_h);
			return (nPos >= 0)? S_OK : E_FAIL;
		}

		HRESULT Flush()
		{
			return ::fflush((FILE *)m_h)? S_OK : E_FAIL;
		}

		HRESULT LockRange(UINT64 nPos, UINT64 nCount)
		{
			return E_FAIL;
		}

		HRESULT UnlockRange(UINT64 nPos, UINT64 nCount)
		{
			return E_FAIL;
		}

		HRESULT SetSize(UINT64 nNewLen)
		{
			return Seek(nNewLen, (UINT32)CFile::Begin);
		}

		HRESULT GetSize(UINT64& nLen) const
		{
			int nOffset = ::ftell((FILE *)m_h);
			if (nOffset < 0)
				return E_FAIL;
			::fseek((FILE *)m_h, 0, SEEK_END);
			nLen = ::ftell((FILE *)m_h);
			::fseek((FILE *)m_h, nOffset, SEEK_SET);
			return S_OK;
		}

		//bool IsEOF()
		//{
		//	return ::feof((FILE *)m_h) > 0;
		//}
	};

}; // namespace LGN

#endif // #ifndef __LGNUNIX_FILE_H__
