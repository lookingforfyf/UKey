#ifndef __LGNUNIX_CORE_H__
#define __LGNUNIX_CORE_H__


namespace LGN
{
	// simple string
	template< typename BaseType = CHAR >
	class ChTraitsBase
	{
	public:
		typedef CHAR XCHAR;
		typedef LPSTR PXSTR;
		typedef LPCSTR PCXSTR;
		typedef WCHAR YCHAR;
		typedef LPWSTR PYSTR;
		typedef LPCWSTR PCYSTR;
	};

	template<>
	class ChTraitsBase< WCHAR >
	{
	public:
		typedef WCHAR XCHAR;
		typedef LPWSTR PXSTR;
		typedef LPCWSTR PCXSTR;
		typedef CHAR YCHAR;
		typedef LPSTR PYSTR;
		typedef LPCSTR PCYSTR;
	};

	template< typename _CharType = CHAR >
	class ChTraitsCRT : public ChTraitsBase< _CharType >
	{
	public:
		static LPSTR CharNext(LPCSTR p) throw()
		{
			LGNASSERT(p != NULL);
			return const_cast<LPSTR>(p+1);
		}

		static int IsDigit(CHAR ch) throw()
		{
			return isdigit(ch);
		}

		static int IsSpace(CHAR ch) throw()
		{
			return isspace(ch);
		}

		static int StringCompare(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			return strcmp((LPCSTR) pstrOne, (LPCSTR) pstrOther);
		}

		static int StringCompareIgnore(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			CTempStrBuff str1(pstrOne);
			CTempStrBuff str2(pstrOther);
			StringUppercase((LPSTR)str1);
			StringUppercase((LPSTR)str2);
			return StringCompare((LPCSTR)str1, (LPCSTR)str2);
		}

		static int StringCollate(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			return strcoll(pstrOne, pstrOther);
		}

		static int StringCollateIgnore(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			CTempStrBuff str1(pstrOne);
			CTempStrBuff str2(pstrOther);
			StringUppercase((LPSTR)str1);
			StringUppercase((LPSTR)str2);
			return StringCollate((LPCSTR)str1, (LPCSTR)str2);
		}

		static LPCSTR StringFindString(LPCSTR pstrBlock, LPCSTR pstrMatch) throw()
		{
			return strstr(pstrBlock, pstrMatch);
		}

		static LPSTR StringFindString(LPSTR pszBlock, LPCSTR pszMatch) throw()
		{
			return( const_cast< LPSTR >( StringFindString( const_cast< LPCSTR >( pszBlock ), pszMatch ) ) );
		}

		static LPCSTR StringFindChar(LPCSTR pszBlock, CHAR chMatch) throw()
		{
			return strchr(pszBlock, chMatch);
		}

		static LPCSTR StringFindCharRev(LPCSTR psz, CHAR ch) throw()
		{
			return strrchr(psz, ch);
		}

		static LPCSTR StringScanSet(LPCSTR pszBlock, LPCSTR pszMatch) throw()
		{
			return strpbrk(pszBlock, pszMatch);
		}

		static int StringSpanIncluding(LPCSTR pstrBlock, LPCSTR pstrSet) throw()
		{
			return strspn(pstrBlock, pstrSet);
		}

		static int StringSpanExcluding(LPCSTR pstrBlock, LPCSTR pstrSet) throw()
		{
			return strcspn(pstrBlock, pstrSet);
		}

		static LPSTR StringUppercase(LPSTR psz) throw()
		{
			int nLen = LGN::API::SafeStringLength(psz);
			for (int i=0; i<nLen; i++)
				psz[i] = toupper(psz[i]);
			return psz;
		}

		static LPSTR StringLowercase(LPSTR psz) throw()
		{
			int nLen = LGN::API::SafeStringLength(psz);
			for (int i=0; i<nLen; i++)
				psz[i] = tolower(psz[i]);
			return psz;
		}

		static LPSTR StringReverse(LPSTR psz) throw()
		{
			int nLen = LGN::API::SafeStringLength(psz);
			for (int i=0,j=nLen-1; i<j; i++,j--)
			{
				CHAR ch = psz[i];
				psz[j] = psz[i];
				psz[i] = ch;
			}
			return psz;
			//return strrev( psz );
		}

		static int GetFormattedLength(LPCSTR pszFormat, va_list args) throw()
		{
			CHAR szBuffer[1028];

			// wvsprintf always truncates the output to 1024 character plus
			// the '\0'.
			int nLength = vsprintf(szBuffer, pszFormat, args);
			LGNASSERT(nLength >= 0);
			LGNASSERT(nLength <= 1024);

			return nLength;
		}

		static int Format(LPSTR pszBuffer, LPCSTR pszFormat, va_list args) throw()
		{
			return ::vsprintf(pszBuffer, pszFormat, args);
		}

		static int GetBaseTypeLength(LPCSTR pszSrc) throw()
		{
			// Returns required buffer length in XCHARs
			return strlen(pszSrc);
		}

		static int GetBaseTypeLength(LPCSTR pszSrc, int nLength) throw()
		{
			(void)pszSrc;
			// Returns required buffer length in XCHARs
			return nLength;
		}

		static int GetBaseTypeLength(LPCWSTR pszSrc) throw()
		{
			// Returns required buffer length in XCHARs
			LgnThrow(E_LGN_NOT_SUPPORTED);
			return -1;
		}

		static int GetBaseTypeLength(LPCWSTR pszSrc, int nLength) throw()
		{
			// Returns required buffer length in XCHARs
			LgnThrow(E_LGN_NOT_SUPPORTED);
			return -1;
		}

		static void ConvertToBaseType(LPSTR pszDest, int nDestLength, LPCSTR pszSrc, int nSrcLength = -1) throw()
		{
			(void)nSrcLength;
			// nLen is in chars
			LGN::API::memcpy(pszDest, pszSrc, nDestLength*sizeof(CHAR));
		}

		static void ConvertToBaseType(LPSTR pszDest, int nDestLength, LPCWSTR pszSrc, int nSrcLength = -1) throw()
		{
			// nLen is in XCHARs
			LgnThrow(E_LGN_NOT_SUPPORTED);
			return ;
		}

		//static void ConvertToOem(LPSTR pstrString) throw()
		//{
		//	::AnsiToOem(pstrString, pstrString);
		//}

		//static void ConvertToAnsi(LPSTR pstrString) throw()
		//{
		//	::OemToAnsi(pstrString, pstrString);
		//}

		static void FloodCharacters(CHAR ch, int nLength, LPSTR pstr) throw()
		{
			// nLength is in XCHARs
			LGN::API::memset(pstr, ch, nLength);
		}

		//static BSTR AllocSysString(const LPSTR pchData, int nDataLength) throw()
		//{
		//	int nLen = ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength,NULL, NULL);
		//	BSTR bstr = ::SysAllocStringLen(NULL, nLen);
		//	if (bstr != NULL)
		//	{
		//		::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, bstr, nLen);
		//	}

		//	return bstr;
		//}

		//static BOOL ReAllocSysString(const LPSTR pchData, BSTR* pbstr, int nDataLength) throw()
		//{
		//	int nLen = ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, NULL, NULL);
		//	BOOL bSuccess =::SysReAllocStringLen(pbstr, NULL, nLen);
		//	if (bSuccess)
		//	{
		//		::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, *pbstr, nLen);
		//	}

		//	return bSuccess;
		//}

		//static DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource,
		//	DWORD dwMessageID, DWORD dwLanguageID, LPSTR pstrBuffer,
		//	DWORD nSize, va_list* pArguments) throw()
		//{
		//	return ::FormatMessageA(dwFlags, lpSource, dwMessageID, dwLanguageID, pstrBuffer, nSize, pArguments);
		//}

		static int SafeStringLen(LPCSTR psz) throw()
		{
			// returns length in bytes
			return LGN::API::SafeStringLength(psz);
		}

		static int SafeStringLen(LPCWSTR psz) throw()
		{
			// returns length in wchar_ts
			return LGN::API::SafeStringLength(psz);
		}

		static int GetCharLen(LPCWSTR) throw()
		{
			// returns CHAR length
			return 1;
		}
		static int GetCharLen(LPCSTR psz) throw()
		{
			return 1;
		}

		//static DWORD GetEnvironmentVariable(LPCSTR pstrVar, LPSTR pstrBuffer, DWORD dwSize) throw()
		//{
		//	return ::GetEnvironmentVariableA(pstrVar, pstrBuffer, dwSize);
		//}
	};

	// specialization for WCHAR
	template<>
	class ChTraitsCRT< WCHAR > : public ChTraitsBase< WCHAR >
	{
	public:
	};


	template< typename _CharType = CHAR >
	class ChTraitsEx : public ChTraitsCRT< _CharType >
	{
	public:
		static int StringLength(LPCSTR pszStr)
		{
			return strlen(pszStr);
		}

		static LPSTR StringCat(LPSTR pszDest, LPCSTR pszSrc)
		{
			return strcat(pszDest, pszSrc);
		}

		static LPSTR StringCopy(LPSTR pszDest, LPCSTR pszSrc)
		{
			return strcpy(pszDest, pszSrc);
		}
		static LPSTR StringCopyN(LPSTR pszDest, LPCSTR pszSrc, int iMaxLength)
		{
			return strncpy(pszDest, pszSrc, iMaxLength);
		}

		static int FormatN(LPSTR lpOut, int cchLimitIn, LPCSTR lpFmt, va_list arglist)
		{
			return vsprintf(lpOut, lpFmt, arglist);
		}
		static int FormatNP(LPSTR lpOut, int cchLimitIn, LPCSTR lpFmt, ...)
		{
			va_list ptr;
			va_start(ptr, lpFmt);
			int nRet = FormatN(lpOut, cchLimitIn, lpFmt, ptr);
			va_end(ptr);
			return nRet;
		}

		static void SOutputDebugString(LPCSTR pszOutputString)
		{
			printf("%s", pszOutputString);
		}

		static UINT32 SFormatMessage(UINT32 dwMessageId, LPSTR lpBuffer, UINT32 nSize)
		{
			return sprintf(lpBuffer,"errno=0x%X", dwMessageId);
		}

		static HANDLE SOpenFileAlways(LPCSTR pszFileName)
		{
			FILE * pf = ::fopen(pszFileName, "a+b");
			return pf;
		}

		static UINT32 SGetModuleFileName(HANDLE hModule, LPSTR pszFileName, UINT32 dwSize)
		{
			if (hModule == NULL)
			{
				return readlink("/proc/self/exe", pszFileName, dwSize);
			}
			else
			{
				pszFileName[0] = 0; // test
				return 0;
			}
		}

		static LPSTR SPathRemoveFileSpec(LPSTR pszPath) // == dirname
		{
			int n = strlen(pszPath);
			for (int i=n-1; i>=0; i--)
			{
				if (pszPath[i] == '/')
				{
					pszPath[i] = 0;
					break;
				}
			}
			return pszPath;
		}

		static LPSTR SPathCombine(LPSTR pszDest, LPCSTR pszDir, LPCSTR pszFile)
		{
			int nDirLen = strlen(pszDir);
			strcpy(pszDest,pszDir);
			if ((nDirLen > 0) && (pszDir[nDirLen-1] != '/'))
				strcat(pszDest, "/");
			strcat(pszDest, pszFile);
			return pszDest;
		}

#ifdef _DLFCN_H
		static LPVOID SLoadLibrary(LPCSTR lpFileName)
		{
			return dlopen(lpFileName, RTLD_NOW);
		}
#endif
	};

	template<>
	class ChTraitsEx< WCHAR > : public ChTraitsCRT< WCHAR >
	{
	public:
	};

	typedef ChTraitsEx< CHAR > ChTraitsExA;
	typedef ChTraitsEx< WCHAR > ChTraitsExW;
	typedef ChTraitsEx< TCHAR > ChTraitsExT;



	// class CModulePathT
	template< typename _BaseType>
	class CLoadModuleT
	{
	public:
		CLoadModuleT() : m_hMod(NULL)
		{
		}
		~CLoadModuleT()
		{
			//Free();
		}
#ifdef _DLFCN_H
		CLoadModuleT(const _BaseType * pszModuleName)
		{
			m_hMod = ChTraitsEx< _BaseType >::SLoadLibrary(pszModuleName);
		}
		bool Load(LPCSTR pszModuleName)
		{
			m_hMod = ChTraitsEx< _BaseType >::SLoadLibrary(pszModuleName);
			return (m_hMod != NULL);
		}

		void Free()
		{
			if (m_hMod != NULL)
			{
				dlclose(m_hMod);
				m_hMod = NULL;
			}
		}

		LPVOID GetAddress(LPCSTR pszProcName)
		{
			return dlsym(m_hMod, pszProcName);
		}
#endif // #ifdef _DLFCN_H
		
		LPVOID Detach()
		{
			LPVOID hMod = m_hMod;
			m_hMod= NULL;
			return hMod;
		}
		
		operator const LPVOID ()
		{
			return m_hMod;
		}

	protected:
		LPVOID m_hMod;

	private:
		CLoadModuleT( const CLoadModuleT& ) throw();
		CLoadModuleT& operator=( const CLoadModuleT& ) throw();
	};

	typedef CLoadModuleT< CHAR > CLoadModuleA;
	typedef CLoadModuleT< WCHAR > CLoadModuleW;
	typedef CLoadModuleT< TCHAR > CLoadModule;


	// class FileTraitsTrace
	template< typename BaseType >
	class FileTraitsTrace
	{
	public:

		static HRESULT Append(const BaseType * pszFileName, LPCVOID pData, UINT32 unLength)
		{
			FILE * pf = (FILE *)ChTraitsEx< BaseType >::SOpenFileAlways(pszFileName);
			if (pf == NULL)
				return E_FAIL;

			::fwrite(pData, 1, unLength, pf);

			::fclose(pf);
			return S_OK;
		}
	};

	typedef FileTraitsTrace< CHAR > FileTraitsTraceA; 
	typedef FileTraitsTrace< WCHAR > FileTraitsTraceW; 
	typedef FileTraitsTrace< TCHAR > FileTraitsTraceT; 

}; // namespace LGN

#endif // #ifndef __LGNUNIX_CORE_H__
