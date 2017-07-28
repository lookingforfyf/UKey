/*! \file lgnwin.core.h 
 *  \brief WIN系统的基本功能封装类
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  主要实现以下功能：
 *  
 *  <ul>
 *  	<li> 一些全局函数（当包含定义ATL文件才有效）
 *		<li> 如果没有包含atlstr.h，则实现ChTraitsBase和ChTraitsOS静态模板类
 *  	<li> 模块路径操作的模板类 CModulePathT
 *  	<li> 加载DLL模块的模板类 CLoadModuleT
 *  	<li> 文件操作的静态模板类 FileTraitsTrace,只用于lgntrace.h
 *  </ul>
 *
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNWIN_CORE_H__
#define __LGNWIN_CORE_H__

#include <shlwapi.h>

//#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////
// Win32 libraries

#ifndef _ATL_NO_DEFAULT_LIBS
#if (_MSC_VER >= 1000)
#pragma comment(lib, "shlwapi.lib")
#endif
#endif  // !_ATL_NO_DEFAULT_LIBS


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// global function

	//! 当工程包含atlconv.h则同<B>ATL::_AtlGetConversionACP</B> 具体参考MSDN；否则返回CP_ACP
	inline UINT WINAPI _LgnGetConversionACP() throw()
	{
#ifdef __ATLCONV_H__
		return ATL::_AtlGetConversionACP();
#else
		return( CP_ACP );
#endif
	}

#ifdef __ATLCORE_H__
	inline BOOL LgnIsValidAddress(const void* p, size_t nBytes, BOOL bReadWrite = TRUE)
	{
		return ATL::AtlIsValidAddress(p, nBytes, bReadWrite);
	}

	inline BOOL LgnIsValidString(LPCSTR psz, size_t nMaxLength = UINT_MAX)
	{
		return ATL::AtlIsValidString(psz, nMaxLength);
	}

	inline BOOL LgnIsValidString(LPCWSTR psz, size_t nMaxLength = UINT_MAX)
	{
		return ATL::AtlIsValidString(psz, nMaxLength);
	}

	inline HINSTANCE LgnFindStringResourceInstance(UINT nID, WORD wLanguage = 0) throw()
	{
		return ATL::AtlFindStringResourceInstance(nID, wLanguage);
	}
#else
	//! 当工程包含atlcore.h则同<B>ATL::AtlIsValidAddress</B> 具体参考MSDN；否则返回TRUE
	inline BOOL LgnIsValidAddress(const void* p, size_t nBytes, BOOL bReadWrite = TRUE)
	{
		return TRUE;
	}

	//! 当工程包含atlcore.h则同<B>ATL::AtlIsValidString</B> 具体参考MSDN；否则返回TRUE
	inline BOOL LgnIsValidString(LPCSTR psz, size_t nMaxLength = -1)
	{
		return TRUE;
	}

	//! 当工程包含atlcore.h则同<B>ATL::AtlIsValidString</B> 具体参考MSDN；否则返回TRUE
	inline BOOL LgnIsValidString(LPCWSTR psz, size_t nMaxLength = -1)
	{
		return TRUE;
	}

	//! 当工程包含atlcore.h则同<B>ATL::AtlFindStringResourceInstance</B> 具体参考MSDN；否则返回NULL
	inline HINSTANCE LgnFindStringResourceInstance(UINT nID, WORD wLanguage = 0) throw()
	{
		return( NULL );
	}
#endif
	

////////////////////////////////////////////////////////////////////////////////////////////////////
// class ChTraitsBase, ChTraitsOS
#ifdef __ATLSTR_H__
#define LGN_CHTRAISTSBASE_NS ATL
#else
#define LGN_CHTRAISTSBASE_NS LGN
	//! 来源 atlsimpstr.h，同<B>ATL::ChTraitsBase</B>，具体参考MSDN
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

	//! 来源 atlsimpstr.h，同<B>ATL::ChTraitsBase</B>，具体参考MSDN
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

	//! 来源 atlstr.h，同<B>ATL::ChTraitsOS</B>，具体参考MSDN
	template< typename _CharType = CHAR >
	class ChTraitsOS : public ChTraitsBase< _CharType >
	{
	public:
		static int tclen(LPCSTR p) throw()
		{
			LGNASSERT(p != NULL);
			LPCSTR pnext = CharNext(p);
			return ((pnext-p)>1) ? 2 : 1;
		}
		static LPCSTR strchr(LPCSTR p, CHAR ch) throw()
		{
			LGNASSERT(p != NULL);
			//strchr for '\0' should succeed
			do
			{
				if (*p == ch)
				{
					return p;
				}
				p = CharNext(p);
			} while( *p != 0 );

			return NULL;
		}
		static LPCSTR strchr_db(LPCSTR p, CHAR ch1, CHAR ch2) throw()
		{
			LGNASSERT(p != NULL);
			while (*p != 0)
			{
				if (*p == ch1 && *(p+1) == ch2)
				{
					return p;
				}
				p = CharNext(p);
			}
			return NULL;
		}
		static LPCSTR strrchr(LPCSTR p, CHAR ch) throw()
		{
			LGNASSERT(p != NULL);
			LPCSTR pch = NULL;
			while (*p != 0)
			{
				if (*p == ch)
					pch = p;
				p = CharNext(p);
			}
			return const_cast< LPSTR >( pch );
		}
		static LPSTR _strrev(LPSTR psz) throw()
		{
			// Optimize NULL, zero-length, and single-CHAR case.
			if ((psz == NULL) || (psz[0] == '\0') || (psz[1] == '\0'))
				return psz;

			LPSTR p = psz;

			while (p[1] != 0) 
			{
				LPSTR pNext = CharNext(p);
				if(pNext > p + 1)
				{
					CHAR p1 = *p;
					*p = *(p + 1);
					*(p + 1) = p1;
				}
				p = pNext;
			}

			LPSTR q = psz;

			while (q < p)
			{
				CHAR t = *q;
				*q = *p;
				*p = t;
				q++;
				p--;
			}
			return psz;
		}
		static LPCSTR strstr(LPCSTR pStr, LPCSTR pCharSet) throw()
		{
			LGNASSERT(pStr != NULL);
			int nLen = ::lstrlenA(pCharSet);
			if (nLen == 0)
				return pStr;

			LPCSTR pMatch;
			LPCSTR pStart = pStr;
			while ((pMatch = strchr(pStart, *pCharSet)) != NULL)
			{
				if (LGN::API::memcmp(pMatch, pCharSet, nLen*sizeof(CHAR)) == 0)
					return pMatch;
				pStart = CharNextA(pMatch);
			}

			return NULL;
		}
		static int strspn(LPCSTR pStr, LPCSTR pCharSet) throw()
		{
			LGNASSERT(pStr != NULL);
			int nRet = 0;
			LPSTR p = const_cast<LPSTR>(pStr);
			while (*p != 0)
			{
				LPSTR pNext = CharNext(p);
				if(pNext > p + 1)
				{
					if(strchr_db(pCharSet, *p, *(p+1)) == NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(strchr(pCharSet, *p) == NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}
		static int strcspn(LPCSTR pStr, LPCSTR pCharSet) throw()
		{
			LGNASSERT(pStr != NULL);
			int nRet = 0;
			LPSTR p = const_cast<LPSTR>(pStr);
			while (*p != 0)
			{
				LPSTR pNext = CharNext(p);
				if(pNext > p + 1)
				{
					if(strchr_db(pCharSet, *p, *(p+1)) != NULL)
						break;
					nRet += 2;
				}
				else
				{
					if(strchr(pCharSet, *p) != NULL)
						break;
					nRet++;
				}
				p = pNext;
			}
			return nRet;
		}
		static LPCSTR strpbrk(LPCSTR p, LPCSTR lpszCharSet) throw()
		{
			LGNASSERT(p != NULL);
			while (*p != 0)
			{
				if (strchr(lpszCharSet, *p) != NULL)
				{
					return p;
				}
				p = CharNext(p);
			}
			return NULL;
		}

		static LPSTR CharNext(LPCSTR p) throw()
		{
			LGNASSERT(p != NULL);
			if (*p == '\0')  // ::CharNextA won't increment if we're at a \0 already
				return const_cast<LPSTR>(p+1);
			else
				return ::CharNextA(p);
		}

		static int IsDigit(CHAR ch) throw()
		{
			WORD type;
			::GetStringTypeExA(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
			return (type & C1_DIGIT) == C1_DIGIT;
		}

		static int IsSpace(CHAR ch) throw()
		{
			WORD type;
			::GetStringTypeExA(GetThreadLocale(), CT_CTYPE1, &ch, 1, &type);
			return (type & C1_SPACE) == C1_SPACE;
		}

		static int StringCompare(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			return ::lstrcmpA((LPCSTR) pstrOne, (LPCSTR) pstrOther);
		}

		static int StringCompareIgnore(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			return ::lstrcmpiA((LPCSTR) pstrOne, (LPCSTR) pstrOther);
		}

		static int StringCollate(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			int nRet = ::CompareStringA(GetThreadLocale(), 0, (LPCSTR)pstrOne, -1, (LPCSTR)pstrOther, -1);
			LGNASSERT(nRet != 0);
			return nRet-2;  // Convert to strcmp convention.  This really is documented.
		}

		static int StringCollateIgnore(LPCSTR pstrOne, LPCSTR pstrOther) throw()
		{
			int nRet = ::CompareStringA(GetThreadLocale(), NORM_IGNORECASE, (LPCSTR)pstrOne, -1, (LPCSTR)pstrOther, -1);
			LGNASSERT(nRet != 0);
			return nRet-2;  // Convert to strcmp convention.  This really is documented.
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
			return ::CharUpperA( psz );
		}

		static LPSTR StringUppercase(LPSTR psz, size_t size) throw()
		{
			DWORD dwSize = static_cast<DWORD>(size);
			::CharUpperBuffA(psz, dwSize);
			return psz;
		}

		static LPSTR StringLowercase(LPSTR psz) throw()
		{
			return ::CharLowerA( psz );
		}

		static LPSTR StringLowercase(LPSTR psz, size_t size) throw()
		{
			::CharLowerBuffA(psz, size);
			return psz;
		}

		static LPSTR StringReverse(LPSTR psz) throw()
		{
			return _strrev( psz );
		}

		static int GetFormattedLength(LPCSTR pszFormat, va_list args) throw()
		{
			CHAR szBuffer[1028];

			// wvsprintf always truncates the output to 1024 character plus
			// the '\0'.
			int nLength = ::wvsprintfA(szBuffer, pszFormat, args);
			LGNASSERT(nLength >= 0);
			LGNASSERT(nLength <= 1024);

			return nLength;
		}

		static int Format(LPSTR pszBuffer, LPCSTR pszFormat, va_list args) throw()
		{
			return ::wvsprintfA(pszBuffer, pszFormat, args);
		}

		static int Format(LPSTR pszBuffer, size_t nLength, LPCSTR pszFormat, va_list args) throw()
		{
			nLength;
			return ::wvsprintfA(pszBuffer, pszFormat, args);
		}

		static int GetBaseTypeLength(LPCSTR pszSrc) throw()
		{
			// Returns required buffer length in XCHARs
			return ::lstrlenA(pszSrc);
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
			return ::WideCharToMultiByte(_LgnGetConversionACP(), 0, pszSrc, -1, NULL, 0, NULL, NULL)-1;
		}

		static int GetBaseTypeLength(LPCWSTR pszSrc, int nLength) throw()
		{
			// Returns required buffer length in XCHARs
			return ::WideCharToMultiByte(_LgnGetConversionACP(), 0, pszSrc, nLength, NULL, 0, NULL, NULL);
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
			::WideCharToMultiByte(_LgnGetConversionACP(), 0, pszSrc, nSrcLength, pszDest, nDestLength, NULL, NULL);
		}

		static void ConvertToOem(LPSTR pstrString) throw()
		{
			::AnsiToOem(pstrString, pstrString);
		}

		static void ConvertToAnsi(LPSTR pstrString) throw()
		{
			::OemToAnsi(pstrString, pstrString);
		}

		static void FloodCharacters(CHAR ch, int nLength, LPSTR pstr) throw()
		{
			// nLength is in XCHARs
			LGN::API::memset(pstr, ch, nLength);
		}

		static BSTR AllocSysString(LPCSTR pchData, int nDataLength) throw()
		{
			int nLen = ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength,NULL, NULL);
			BSTR bstr = ::SysAllocStringLen(NULL, nLen);
			if (bstr != NULL)
			{
				::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, bstr, nLen);
			}

			return bstr;
		}

		static BOOL ReAllocSysString(LPCSTR pchData, BSTR* pbstr, int nDataLength) throw()
		{
			int nLen = ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, NULL, NULL);
			BOOL bSuccess =::SysReAllocStringLen(pbstr, NULL, nLen);
			if (bSuccess)
			{
				::MultiByteToWideChar(_LgnGetConversionACP(), 0, pchData, nDataLength, *pbstr, nLen);
			}

			return bSuccess;
		}

		static DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource,
			DWORD dwMessageID, DWORD dwLanguageID, LPSTR pstrBuffer,
			DWORD nSize, va_list* pArguments) throw()
		{
			return ::FormatMessageA(dwFlags, lpSource, dwMessageID, dwLanguageID, pstrBuffer, nSize, pArguments);
		}

		static int SafeStringLen(LPCSTR psz) throw()
		{
			// returns length in bytes
			return (psz != NULL) ? lstrlenA(psz) : 0;
		}

		static int SafeStringLen(LPCWSTR psz) throw()
		{
			// returns length in wchar_ts
			return (psz != NULL) ? lstrlenW(psz) : 0;
		}

		static int GetCharLen(LPCWSTR) throw()
		{
			// returns CHAR length
			return 1;
		}
		static int GetCharLen(LPCSTR psz) throw()
		{
			LPCSTR p = ::CharNextA(psz);
			return (p - psz);
		}

		static DWORD GetEnvironmentVariable(LPCSTR pstrVar, LPSTR pstrBuffer, DWORD dwSize) throw()
		{
			return ::GetEnvironmentVariableA(pstrVar, pstrBuffer, dwSize);
		}
	};

	// specialization for WCHAR
	//! 来源 atlstr.h，同<B>ATL::ChTraitsOS</B>，具体参考MSDN
	template<>
	class ChTraitsOS< WCHAR > : public ChTraitsBase< WCHAR >
	{
	protected:
//#if defined(_UNICODE) && !defined(_CSTRING_ALWAYS_THUNK)
		static int CompareStringW(LCID lcid, DWORD dwFlags, LPCWSTR pszString1, int nLength1, LPCWSTR pszString2, int nLength2)
		{
			return ::CompareStringW(lcid, dwFlags, pszString1, nLength1,  pszString2, nLength2);
		}
		static BOOL GetStringTypeExW(LCID lcid, DWORD dwInfoType, LPCWSTR pszSrc, int nLength, LPWORD pwCharType)
		{
			return ::GetStringTypeExW(lcid, dwInfoType, pszSrc, nLength, pwCharType);
		}
		static int lstrcmpiW(LPCWSTR psz1, LPCWSTR psz2)
		{
			return ::lstrcmpiW(psz1, psz2);
		}
		static LPWSTR CharLowerW(LPWSTR psz)
		{
			return ::CharLowerW(psz);
		}
		static LPWSTR CharUpperW(LPWSTR psz)
		{
			return ::CharUpperW(psz);
		}
		static DWORD _GetEnvironmentVariableW(LPCWSTR pszName, LPWSTR pszBuffer, DWORD nSize)
		{
			return ::GetEnvironmentVariableW(pszName, pszBuffer, nSize);
		}
//#endif  // !_UNICODE

	public:
		static int tclen(LPCWSTR) throw()
		{
			return 1;
		}
		static LPCWSTR strchr(LPCWSTR p, WCHAR ch) throw()
		{
			//strchr for '\0' should succeed
			while (*p != 0)
			{
				if (*p == ch)
				{
					return p;
				}
				p++;
			}
			return (*p == ch) ? p : NULL;
		}
		static LPCWSTR strrchr(LPCWSTR p, WCHAR ch) throw()
		{
			LPCWSTR pch = p+::lstrlenW(p);
			while ((pch != p) && (*pch != ch))
			{
				pch--;
			}
			if (*pch == ch)
			{
				return pch;
			}
			else
			{
				return NULL;
			}
		}
		static LPWSTR _strrev(LPWSTR psz) throw()
		{
			// Optimize NULL, zero-length, and single-CHAR case.
			if ((psz == NULL) || (psz[0] == L'\0') || (psz[1] == L'\0'))
				return psz;

			LPWSTR p = psz+(::lstrlenW( psz )-1);
			LPWSTR q = psz;
			while(q < p)
			{
				WCHAR t = *q;
				*q = *p;
				*p = t;
				q++;
				p--;
			}
			return psz;
		}
		static LPCWSTR strstr(LPCWSTR pStr, LPCWSTR pCharSet) throw()
		{
			int nLen = ::lstrlenW(pCharSet);
			if (nLen == 0)
				return pStr;

			LPCWSTR pMatch;
			LPCWSTR pStart = pStr;
			while ((pMatch = strchr(pStart, *pCharSet)) != NULL)
			{
				if (LGN::API::memcmp(pMatch, pCharSet, nLen*sizeof(WCHAR)) == 0)
					return pMatch;
				pStart++;
			}

			return NULL;
		}
		static int strspn(LPCWSTR psz, LPCWSTR pszCharSet) throw()
		{
			int nRet = 0;
			LPCWSTR p = psz;
			while (*p != 0)
			{
				if(strchr(pszCharSet, *p) == NULL)
					break;
				nRet++;
				p++;
			}
			return nRet;
		}
		static int strcspn(LPCWSTR psz, LPCWSTR pszCharSet) throw()
		{
			int nRet = 0;
			LPCWSTR p = psz;
			while (*p != 0)
			{
				if(strchr(pszCharSet, *p) != NULL)
					break;
				nRet++;
				p++;
			}
			return nRet;
		}
		static LPCWSTR strpbrk(LPCWSTR psz, LPCWSTR pszCharSet) throw()
		{
			LPCWSTR p = psz;
			while (*p != 0)
			{
				if (strchr(pszCharSet, *p) != NULL)
					return p;
				p++;
			}
			return NULL;
		}

		static LPWSTR CharNext(LPCWSTR p) throw()
		{
			return const_cast< LPWSTR >( p+1 );
		}

		static int IsDigit(WCHAR ch) throw()
		{
			WORD type;
			::GetStringTypeExW(0, CT_CTYPE1, &ch, 1, &type);
			return (type & C1_DIGIT) == C1_DIGIT;
		}

		static int IsSpace(WCHAR ch) throw()
		{
			WORD type;
			::GetStringTypeExW(0, CT_CTYPE1, &ch, 1, &type);
			return (type & C1_SPACE) == C1_SPACE;
		}


		static int StringCompare(LPCWSTR pstrOne, LPCWSTR pstrOther) throw()
		{
			//return wcscmp(pstrOne, pstrOther); // ??
			return ::lstrcmpW(pstrOne, pstrOther);
		}

		static int StringCompareIgnore(LPCWSTR pstrOne, LPCWSTR pstrOther) throw()
		{
			return ::lstrcmpiW(pstrOne, pstrOther);
		}

		static int StringCollate(LPCWSTR pstrOne, LPCWSTR pstrOther) throw()
		{ 
			int nRet = CompareStringW(GetThreadLocale(), 0, pstrOne, -1, pstrOther, -1);
			LGNASSERT(nRet != 0);
			return nRet-2;  // Convert to strcmp convention.  This really is documented.
		}

		static int StringCollateIgnore(LPCWSTR pstrOne, LPCWSTR pstrOther) throw()
		{
			int nRet = CompareStringW(GetThreadLocale(), NORM_IGNORECASE, pstrOne, -1, pstrOther, -1);
			LGNASSERT(nRet != 0);
			return nRet-2;  // Convert to strcmp convention.  This really is documented.
		}

		static LPCWSTR StringFindString(LPCWSTR pstrBlock, LPCWSTR pstrMatch) throw()
		{
			return strstr(pstrBlock, pstrMatch);
		}

		//static LPWSTR StringFindString( LPWSTR pszBlock, LPCWSTR pszMatch ) throw()
		//{
		//	return( const_cast< LPWSTR >( StringFindString( const_cast< LPCWSTR >( pszBlock ), pszMatch ) ) );
		//}

		static LPCWSTR StringFindChar(LPCWSTR pstrBlock, WCHAR pstrMatch) throw()
		{
			return strchr(pstrBlock, pstrMatch);
		}

		static LPCWSTR StringFindCharRev(LPCWSTR pstr, WCHAR ch) throw()
		{
			return strrchr(pstr, ch);
		}

		static LPCWSTR StringScanSet(LPCWSTR pszBlock, LPCWSTR pszMatch) throw()
		{
			return strpbrk(pszBlock, pszMatch);
		}

		static int StringSpanIncluding(LPCWSTR pszBlock,
			LPCWSTR pszSet) throw()
		{
			return strspn(pszBlock, pszSet);
		}

		static int StringSpanExcluding(LPCWSTR pszBlock,
			LPCWSTR pszSet) throw()
		{
			return strcspn(pszBlock, pszSet);
		}

		static LPWSTR StringUppercase(LPWSTR psz) throw()
		{
			::CharUpperW(psz);
			return psz;
		}

		static LPWSTR StringUppercase(LPWSTR psz, size_t size) throw()
		{
			DWORD dwSize = static_cast<DWORD>(size);
			::CharUpperBuffW(psz, dwSize);
			return psz;
		}

		static LPWSTR StringLowercase(LPWSTR psz) throw()
		{
			::CharLowerW(psz);
			return psz;
		}

		static LPWSTR StringLowercase(LPWSTR psz, size_t size) throw()
		{
			DWORD dwSize = static_cast<DWORD>(size);
			::CharLowerBuffW(psz, dwSize);
			return psz;
		}

		static LPWSTR StringReverse(LPWSTR psz) throw()
		{
			return _strrev(psz);
		}

//#ifdef _UNICODE
		static int GetFormattedLength(LPCWSTR pszFormat, va_list args) throw()
		{
			WCHAR szBuffer[1028];

			// wvsprintf always truncates the output to 1024 character plus
			// the '\0'.
			int nLength = wvsprintfW(szBuffer, pszFormat, args);
			LGNASSERT(nLength >= 0);
			LGNASSERT(nLength <= 1024);

			return nLength;
		}

		static int Format(LPWSTR pszBuffer, LPCWSTR pszFormat, va_list args) throw()
		{
			return ::wvsprintfW(pszBuffer, pszFormat, args);
		}

		static int Format(LPWSTR pszBuffer, size_t nLength, LPCWSTR pszFormat, va_list args) throw()
		{
			nLength;
			return ::wvsprintfW(pszBuffer, pszFormat, args);
		}
//#endif

		static int GetBaseTypeLength(LPCSTR pszSrc) throw()
		{
			// Returns required buffer size in wchar_ts
			return ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pszSrc, -1, NULL, 0)-1;
		}

		static int GetBaseTypeLength(LPCSTR pszSrc, int nLength) throw()
		{
			// Returns required buffer size in wchar_ts
			return ::MultiByteToWideChar(_LgnGetConversionACP(), 0, pszSrc, nLength, NULL, 0);
		}

		static int GetBaseTypeLength(LPCWSTR pszSrc) throw()
		{
			// Returns required buffer size in wchar_ts
			return ::lstrlenW(pszSrc);
		}

		static int GetBaseTypeLength(LPCWSTR pszSrc, int nLength) throw()
		{
			(void)pszSrc;
			// Returns required buffer size in wchar_ts
			return nLength;
		}

		static void ConvertToBaseType(LPWSTR pszDest, int nDestLength, LPCSTR pszSrc, int nSrcLength = -1) throw()
		{
			// nLen is in wchar_ts
			::MultiByteToWideChar(_LgnGetConversionACP(), 0, pszSrc, nSrcLength, pszDest, nDestLength);
		}

		static void ConvertToBaseType(LPWSTR pszDest, int nDestLength, LPCWSTR pszSrc, int nSrcLength = -1) throw()
		{
			(void)nSrcLength;
			// nLen is in wchar_ts
			LGN::API::memcpy(pszDest, pszSrc, nDestLength*sizeof(WCHAR));
		}

		// this conversion on Unicode strings makes no sense
		/*
		static void ConvertToOem(LPWSTR)
		{
		LGNASSERT(FALSE);
		}
		*/

		// this conversion on Unicode strings makes no sense
		/*
		static void ConvertToAnsi(LPWSTR)
		{
		LGNASSERT(FALSE);
		}
		*/

		static void FloodCharacters(WCHAR ch, int nLength, LPWSTR pstr) throw()
		{
			// nLength is in XCHARs
			for (int i = 0; i < nLength; i++)
				pstr[i] = ch;
		}

		static BSTR AllocSysString(LPCWSTR pchData, int nDataLength) throw()
		{
			BSTR bstr = ::SysAllocStringLen(pchData, nDataLength);
			return bstr;
		}

		static BOOL ReAllocSysString(LPCWSTR pchData, BSTR* pbstr, int nDataLength) throw()
		{
			return ::SysReAllocStringLen(pbstr, pchData, nDataLength);
		}

//#ifdef _UNICODE
		static DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource,
			DWORD dwMessageID, DWORD dwLanguageID, LPWSTR pstrBuffer,
			DWORD nSize, va_list* pArguments) throw()
		{
			return ::FormatMessageW(dwFlags, lpSource, dwMessageID, dwLanguageID, pstrBuffer, nSize, pArguments);
		}
//#endif
		static int SafeStringLen(const CHAR* psz) throw()
		{
			// returns length in bytes
			return (psz != NULL) ? ::lstrlenA(psz) : 0;
		}

		static int SafeStringLen(LPCWSTR psz) throw()
		{
			// returns length in wchar_ts
			return (psz != NULL) ? ::lstrlenW(psz) : 0;
		}

		static int GetCharLen(LPCWSTR) throw()
		{
			// returns CHAR length
			return 1;
		}
		static int GetCharLen(LPCSTR psz) throw()
		{
			LPCSTR p = ::CharNextA( psz );
			return int( p-psz );
		}

		static DWORD GetEnvironmentVariable(LPCWSTR pstrVar, LPWSTR pstrBuffer, DWORD dwSize) throw()
		{
			return ::GetEnvironmentVariableW(pstrVar, pstrBuffer, dwSize);
		}
	};
#endif // #ifdef __ATLSTR_H__


////////////////////////////////////////////////////////////////////////////////////////////////////
// class ChTraitsEx

	//! ANSI字符串操作相关API封装的静态模板类
	/*! 继承于ChTraitsOS，实现常用的字符串操作相关API（ChTraitsOS未涉及） */
	template< typename _CharType = CHAR >
	class ChTraitsEx : public LGN_CHTRAISTSBASE_NS::ChTraitsOS< _CharType >
	{
	public:
		//! 获取指定的字符串长度（不包括终止空字符）
		/*! 调用<B>lstrlenA</B>，具体参考MSDN
			\param[in] pszStr 源字符串
			\return 字符串的长度。如果pszStr为NULL，则函数返回0。
		*/
		static int StringLength(LPCSTR pszStr)
		{
			return ::lstrlenA(pszStr);
		}

		//! 追加一个字符串到另一个
		/*! 调用<B>lstrcatA</B>，具体参考MSDN
			\param[in,out] pszDest 目标缓冲区。这个缓冲区必须足够大，以包含两个字符串
			\param[in] pszSrc 源字符串，被追加到在\a pszDest参数指定的字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
		*/
		static LPSTR StringCat(LPSTR pszDest, LPCSTR pszSrc)
		{
			return ::lstrcatA(pszDest, pszSrc);
		}

		//! 从一个字符串拷贝到另一个字符串缓冲区
		/*! 调用<B>lstrcpyA</B>，具体参考MSDN
			\param[out] pszDest 目标缓冲区，存放指向的\a pszSrc参数。缓冲区必须足够大以包含字符串，包括终止空字符。
			\param[in] pszSrc 源字符串，被拷贝到在\a pszDest参数指定的字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
			\see 若要复制指定数目的字符 StringCopyN
		*/		
		static LPSTR StringCopy(LPSTR pszDest, LPCSTR pszSrc)
		{
			return ::lstrcpyA(pszDest, pszSrc);
		}
		//! 从一个字符串拷贝指定数目的字符到另一个字符串缓冲区
		/*! 调用<B>lstrcpynA</B>，具体参考MSDN
			\param[out] pszDest 目标缓冲区，它接收复制的字符。该缓冲区必须足够大，能包含指定\a iMaxLength个数的字符，包括一个终止空字符。
			\param[in] pszSrc 源字符串
			\param[in] iMaxLength 指向从字符串缓冲区pszSrc复制到pszDest的最大长度，包括终止空字符
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。即使是\a iMaxLength比源字符串长度更大，该函数仍执行成功。如果函数失败，返回值是NULL。
		*/		
		static LPSTR StringCopyN(LPSTR pszDest, LPCSTR pszSrc, int iMaxLength)
		{
			return ::lstrcpynA(pszDest, pszSrc, iMaxLength);
		}

		//! 用参数列表格式化出一个printf风格的字符串
		/*! 调用<B>wvnsprintfA(vs2003) / wvsprintfA</B>，具体参考MSDN
			\param[out] lpOut 一个缓冲区来保存输出字符串
			\param[in] cchLimitIn 参数\a lpOut允许的最大字符数
			\param[in] lpFmt printf风格的格式字符串
			\param[in] arglist 参数列表
			\return 如果函数成功，返回写入到缓冲区\a lpOut的字符数，不包括任何终止字符。如果出现错误，返回一个负值。
		*/				
		static int FormatN(LPSTR lpOut, int cchLimitIn, LPCSTR lpFmt, va_list arglist)
		{
#if (_MSC_VER >= 1300)
			return ::wvnsprintfA(lpOut, cchLimitIn, lpFmt, arglist);
#else
			return ::wvsprintfA(lpOut, lpFmt, arglist);
#endif
		}
		/*! \brief 格式的数据写入到指定的缓冲区
			\param[out] lpOut 一个缓冲区来保存输出字符串
			\param[in] cchLimitIn 参数\a lpOut允许的最大字符数
			\param[in] lpFmt printf风格的格式字符串
			\return 如果函数成功，返回值是存储在输出缓冲区的字符数，不计终止字符。如果函数失败，返回值是超出预期的输出长度。			
			\see 创建参数列表后调用FormatN
			\note 任何参数转换并复制到输出缓冲区按照相应的格式字符串格式规范。
		*/			
		static int FormatNP(LPSTR lpOut, int cchLimitIn, LPCSTR lpFmt, ...)
		{
			va_list ptr;
			va_start(ptr, lpFmt);
			int nRet = FormatN(lpOut, cchLimitIn, lpFmt, ptr);
			va_end(ptr);
			return nRet;
		}

		//! 向调试器发送字符串
		/*! 调用<B>OutputDebugStringA</B>，具体参考MSDN
			\param[in] pszOutputString 字符串
			\note 在函数名前加'S'为了防止Window宏替换
		*/
		static void SOutputDebugString(LPCSTR pszOutputString)
		{
			::OutputDebugStringA(pszOutputString);
		}

		//! 消息字符串格式化
		/*! 调用<B>FormatMessageA</B>，具体参考MSDN
			\param[in] dwMessageId 错误消息号
			\param[out] lpBuffer 目的字符串
			\param[out] nSize 字符串\a lpBuffer
		*/
		static UINT32 SFormatMessage(UINT32 dwMessageId, LPSTR lpBuffer, UINT32 nSize)
		{
			return ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpBuffer, nSize, NULL);
		}

		//! 总是打开文件，并指向文件尾部，如果文件不存在则创建
		/*! 调用<B>CreateFileA</B>，具体参考MSDN
			\param[in] pszFileName 文件名
			\return HRESULT
		*/
		static HANDLE SOpenFileAlways(LPCSTR pszFileName)
		{
			HANDLE hFile = ::CreateFileA(pszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
				::SetFilePointer(hFile, 0, NULL, FILE_END);
			return hFile;
		}

		//! 获取指定模块完整名称
		/*! 调用<B>GetModuleFileNameA</B>，具体参考MSDN
			\param[in] hModule 模块句柄
			\param[in] lpFileName 一个缓冲区来保存输出字符串
			\param[in] dwSize 字符串缓冲区\a lpFileName的最大长度
			\return 如果函数成功，返回值是字符串长度
			\note 模块句柄\a hModule = NULL时，获取当前进程的执行文件完整名称
		*/
		static UINT32 SGetModuleFileName(HANDLE hModule, LPSTR lpFileName, UINT32 dwSize)
		{
			return ::GetModuleFileNameA((HMODULE)hModule, lpFileName, dwSize);
		}

		//! 如果路径中有文件名则移除它
		/*! 调用<B>PathRemoveFileSpecA</B>，具体参考MSDN
			\param[in,out] pszPath 一个路径（最大长度为MAX_PATH），输出不包含文件名
			\return 如果函数成功，返回值是一个指向缓冲区\a pszPath的指针。如果函数失败，返回值是NULL
		*/
		static LPSTR SPathRemoveFileSpec(LPSTR pszPath)
		{
			if (::PathRemoveFileSpecA(pszPath))
				return pszPath;
			return NULL;
		}

		//! 把两个字符串组合成一个路径
		/*! 调用<B>PathCombineA</B>，具体参考MSDN
			\param[out] pszDest 目的缓冲区，保持输出pszDir和pszFile组合的路径字符串
			\param[in] pszDir 一个目录字符串
			\param[in] pszFile 一个文件名字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
		*/
		static LPSTR SPathCombine(LPSTR pszDest, LPCSTR pszDir, LPCSTR pszFile)
		{
			return ::PathCombineA(pszDest, pszDir, pszFile);
		}

		//! 把指定的模块加载到调用进程的地址空间。
		/*! 调用<B>LoadLibraryA</B>，具体参考MSDN
			\param[in] lpFileName 一个模块的名称。这可以是一个库模块（dll文件）或一个可执行模块（exe文件）。
			\return 如果函数成功，返回值是一个指向模块的句柄。如果函数失败，返回值是NULL。
		*/
		static HMODULE SLoadLibrary(LPCSTR lpFileName)
		{
			return ::LoadLibraryA(lpFileName);
		}
		
	};

	//! UNICODE字符串操作相关API封装的静态模板类
	/*! 继承于ChTraitsOS，实现常用的字符串操作相关API（ChTraitsOS未涉及） */
	template<>
	class ChTraitsEx< WCHAR > : public LGN_CHTRAISTSBASE_NS::ChTraitsOS< WCHAR >
	{
	public:
		//! 获取指定的字符串长度（不包括终止空字符）
		/*! 调用<B>lstrlenW</B>，具体参考MSDN
			\param[in] pszStr 源字符串
			\return 字符串的长度。如果pszStr为NULL，则函数返回0。
		*/
		static int StringLength(LPCWSTR pszStr)
		{
			return ::lstrlenW(pszStr);
		}

		//! 追加一个字符串到另一个
		/*! 调用<B>lstrcatW</B>，具体参考MSDN
			\param[in,out] pszDest 目标缓冲区。这个缓冲区必须足够大，以包含两个字符串
			\param[in] pszSrc 源字符串，被追加到在\a pszDest参数指定的字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
		*/
		static LPWSTR StringCat(LPWSTR pszDest, LPCWSTR pszSrc)
		{
			return ::lstrcatW(pszDest, pszSrc);
		}

		//! 从一个字符串拷贝到另一个字符串缓冲区
		/*! 调用<B>lstrcpyW</B>，具体参考MSDN
			\param[out] pszDest 目标缓冲区，存放指向的\a pszSrc参数。缓冲区必须足够大以包含字符串，包括终止空字符。
			\param[in] pszSrc 源字符串，被拷贝到在\a pszDest参数指定的字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
			\see 若要复制指定数目的字符 StringCopyN
		*/		
		static LPWSTR StringCopy(LPWSTR pszDest, LPCWSTR pszSrc)
		{
			return ::lstrcpyW(pszDest, pszSrc);
		}
		//! 从一个字符串拷贝指定数目的字符到另一个字符串缓冲区
		/*! 调用<B>lstrcpynW</B>，具体参考MSDN
			\param[out] pszDest 目标缓冲区，它接收复制的字符。该缓冲区必须足够大，能包含指定\a iMaxLength个数的字符，包括一个终止空字符。
			\param[in] pszSrc 源字符串
			\param[in] iMaxLength 指向从字符串缓冲区pszSrc复制到pszDest的最大长度，包括终止空字符
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。即使是\a iMaxLength比源字符串长度更大，该函数仍执行成功。如果函数失败，返回值是NULL。
		*/		
		static LPWSTR StringCopyN(LPWSTR pszDest, LPCWSTR pszSrc, int iMaxLength)
		{
			return ::lstrcpynW(pszDest, pszSrc, iMaxLength);
		}

		//! 用参数列表格式化出一个printf风格的字符串
		/*! 调用<B>wvnsprintfW(vs2003) / wvsprintfW</B>，具体参考MSDN
			\param[out] lpOut 一个缓冲区来保存输出字符串
			\param[in] cchLimitIn 参数\a lpOut允许的最大字符数
			\param[in] lpFmt printf风格的格式字符串
			\param[in] arglist 参数列表
			\return 如果函数成功，返回写入到缓冲区\a lpOut的字符数，不包括任何终止字符。如果出现错误，返回一个负值。
		*/				
		static int FormatN(LPWSTR lpOut, int cchLimitIn, LPCWSTR lpFmt, va_list arglist)
		{
#if (_MSC_VER >= 1300)
			return ::wvnsprintfW(lpOut, cchLimitIn, lpFmt, arglist);
#else
			return ::wvsprintfW(lpOut, lpFmt, arglist);
#endif
		}
		/*! \brief 格式的数据写入到指定的缓冲区
			\param[out] lpOut 一个缓冲区来保存输出字符串
			\param[in] cchLimitIn 参数\a lpOut允许的最大字符数
			\param[in] lpFmt printf风格的格式字符串
			\return 如果函数成功，返回值是存储在输出缓冲区的字符数，不计终止字符。如果函数失败，返回值是超出预期的输出长度。			
			\see 创建参数列表后调用FormatN
			\note 任何参数转换并复制到输出缓冲区按照相应的格式字符串格式规范。
		*/			
		static int FormatNP(LPWSTR lpOut, int cchLimitIn, LPCWSTR lpFmt, ...)
		{
			va_list ptr;
			va_start(ptr, lpFmt);
			int nRet = FormatN(lpOut, cchLimitIn, lpFmt, ptr);
			va_end(ptr);
			return nRet;
		}

		//! 向调试器发送字符串
		/*! 调用<B>OutputDebugStringW</B>，具体参考MSDN
			\param[in] pszOutputString 字符串
			\note 在函数名前加'S'为了防止Window宏替换
		*/
		static void SOutputDebugString(LPCWSTR pszOutputString)
		{
			::OutputDebugStringW(pszOutputString);
		}

		//! 消息字符串格式化
		/*! 调用<B>FormatMessageW</B>，具体参考MSDN
			\param[in] dwMessageId 错误消息号
			\param[out] lpBuffer 目的字符串
			\param[out] nSize 字符串\a lpBuffer
		*/
		static UINT32 SFormatMessage(UINT32 dwMessageId, LPWSTR lpBuffer, UINT32 nSize)
		{
			return ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwMessageId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpBuffer, nSize, NULL);
		}

		//! 总是打开文件，并指向文件尾部，如果文件不存在则创建
		/*! 调用<B>CreateFileW</B>，具体参考MSDN
			\param[in] pszFileName 文件名
			\return HRESULT
		*/
		static HANDLE SOpenFileAlways(LPCWSTR pszFileName)
		{
			HANDLE hFile = ::CreateFileW(pszFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
				::SetFilePointer(hFile, 0, NULL, FILE_END);
			return hFile;
		}

		//! 获取指定模块完整名称
		/*! 调用<B>GetModuleFileNameW</B>，具体参考MSDN
			\param[in] hModule 模块句柄
			\param[in] lpFileName 一个缓冲区来保存输出字符串
			\param[in] dwSize 字符串缓冲区\a lpFileName的最大长度
			\return 如果函数成功，返回值是字符串长度
			\note 模块句柄\a hModule = NULL时，获取当前进程的执行文件完整名称
		*/
		static UINT32 SGetModuleFileName(HANDLE hModule, LPWSTR lpFileName, UINT32 dwSize)
		{
			return ::GetModuleFileNameW((HMODULE)hModule, lpFileName, dwSize);
		}

		//! 如果路径中有文件名则移除它
		/*! 调用<B>PathRemoveFileSpecW</B>，具体参考MSDN
			\param[in,out] pszPath 一个路径（最大长度为MAX_PATH），输出不包含文件名
			\return 如果函数成功，返回值是一个指向缓冲区\a pszPath的指针。如果函数失败，返回值是NULL
		*/
		static LPWSTR SPathRemoveFileSpec(LPWSTR pszPath)
		{
			if (::PathRemoveFileSpecW(pszPath))
				return pszPath;
			return NULL;
		}

		//! 把两个字符串组合成一个路径
		/*! 调用<B>PathCombineW</B>，具体参考MSDN
			\param[out] pszDest 目的缓冲区，保持输出pszDir和pszFile组合的路径字符串
			\param[in] pszDir 一个目录字符串
			\param[in] pszFile 一个文件名字符串
			\return 如果函数成功，返回值是一个指向缓冲区\a pszDest的指针。如果函数失败，返回值是NULL
		*/
		static LPWSTR SPathCombine(LPWSTR pszDest, LPCWSTR pszDir, LPCWSTR pszFile)
		{
			return ::PathCombineW(pszDest, pszDir, pszFile);
		}

		//! 把指定的模块加载到调用进程的地址空间。
		/*! 调用<B>LoadLibraryW</B>，具体参考MSDN
			\param[in] lpFileName 一个模块的名称。这可以是一个库模块（dll文件）或一个可执行模块（exe文件）。
			\return 如果函数成功，返回值是一个指向模块的句柄。如果函数失败，返回值是NULL。
		*/
		static HMODULE SLoadLibrary(LPCWSTR lpFileName)
		{
			return ::LoadLibraryW(lpFileName);
		}

	};

	typedef ChTraitsEx< CHAR > ChTraitsExA; //!< 类型定义 LGN::ChTraitsEx< CHAR >
	typedef ChTraitsEx< WCHAR > ChTraitsExW; //!< 类型定义 LGN::ChTraitsEx< WCHAR >
	typedef ChTraitsEx< TCHAR > ChTraitsExT; //!< 类型定义 LGN::ChTraitsEx< TCHAR >



////////////////////////////////////////////////////////////////////////////////////////////////////
// class CLoadModuleT

	//! 加载DLL模块的模板类
	/*! 本类不继承，其对象不可复制 
		-# 点击这里查看 \ref loadmodule_example_page
	*/
	template< typename _BaseType>
	class CLoadModuleT
	{
	public:
		/*! \brief 构造函数，设置模块句柄为NULL						
		*/
		CLoadModuleT() : m_hMod(NULL)
		{
		}
		/*! \brief 构造函数，把指定的模块加载到调用进程的地址空间。				
			\param[in] pszModuleName 一个模块的名称。这可以是一个库模块（dll文件）或一个可执行模块（exe文件）。
		*/
		CLoadModuleT(const _BaseType * pszModuleName)
		{
			m_hMod = ChTraitsEx< _BaseType >::LoadLibraryHandle(pszModuleName);
		}
		//! 虚构函数
		~CLoadModuleT()
		{
			//Free();
		}

		/*! \brief 把指定的模块加载到调用进程的地址空间。				
			\param[in] pszModuleName 一个模块的名称。这可以是一个库模块（dll文件）或一个可执行模块（exe文件）。
		*/
		bool Load(const _BaseType * pszModuleName)
		{
			m_hMod = ChTraitsEx< _BaseType >::SLoadLibrary(pszModuleName);
			return (m_hMod != NULL);
		}

		/*! \brief 释放已加载的动态链接库模块
		*/
		void Free()
		{
			if (m_hMod != NULL)
			{
				::FreeLibrary(m_hMod);
				m_hMod = NULL;
			}
		}

		/*! \brief 获取已加载的模块句柄，并设置内部句柄为NULL
		*/
		HMODULE Detach()
		{
			HMODULE hMod = m_hMod;
			m_hMod= NULL;
			return hMod;
		}

		/*! \brief 操作符类型					
			\return 获取已加载的模块句柄
		*/
		operator const HMODULE()
		{
			return m_hMod;
		}

		/*! \brief 从本加载的模块里检索的一个导出函数或变量的地址。			
			\param[in] pszProcName 一个函数或变量名，或函数的序号值。
			\return 如果函数成功，返回值是一个导出函数或变量的地址。如果函数失败，返回值是NULL。
		*/
		LPVOID GetAddress(LPCSTR pszProcName)
		{
			return ::GetProcAddress(m_hMod, pszProcName);
		}

	protected:
		//! 模块句柄
		HMODULE m_hMod;

	private:
		CLoadModuleT( const CLoadModuleT& ) throw();
		CLoadModuleT& operator=( const CLoadModuleT& ) throw();
	};

	typedef CLoadModuleT< CHAR > CLoadModuleA; //!< 类型定义 LGN::CLoadModuleT< CHAR >
	typedef CLoadModuleT< WCHAR > CLoadModuleW; //!< 类型定义 LGN::CLoadModuleT< WCHAR >
	typedef CLoadModuleT< TCHAR > CLoadModule; //!< 类型定义 LGN::CLoadModuleT< TCHAR >

/*! \page loadmodule_example_page CLoadModuleT例子
<pre>	%CLoadModuleT类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	LGN::CLoadModule loadMod(TEXT("user32.dll"));
	LPVOID pfn = loadMod.GetAddress("SetWindowTextA");
</pre>
*/


////////////////////////////////////////////////////////////////////////////////////////////////////
// class FileTraitsTrace

	//! 文件操作的静态模板类
	/*! 只现实向文件尾部增加数据的功能，只有用于lgntrace.h。 
	*/
	template< typename BaseType >
	class FileTraitsTrace
	{
	public:

		/*! \brief 打开文件并向文件尾部增加数据
			\param[in] pszFileName 文件名
			\param[in] pData 待写的数据
			\param[in] unLength 数据\a pData的长度
			\return HRESULT
		*/
		static HRESULT Append(const BaseType * pszFileName, LPCVOID pData, UINT32 unLength)
		{
			HANDLE hFile = ChTraitsEx< BaseType >::SOpenFileAlways(pszFileName);
			if (hFile == INVALID_HANDLE_VALUE)
				return API::HResultFromError( ::GetLastError() );

			DWORD dwBytesWritten;
			::WriteFile(hFile, pData, unLength, &dwBytesWritten, NULL);

			::CloseHandle(hFile);
			return S_OK;
		}
	};

	typedef FileTraitsTrace< CHAR > FileTraitsTraceA; //!< 类型定义 LGN::FileTraitsTrace< CHAR >
	typedef FileTraitsTrace< WCHAR > FileTraitsTraceW; //!< 类型定义 LGN::FileTraitsTrace< WCHAR >
	typedef FileTraitsTrace< TCHAR > FileTraitsTraceT; //!< 类型定义 LGN::FileTraitsTrace< TCHAR >

}; // namespace LGN

#endif // #ifndef __LGNWIN_CORE_H__
