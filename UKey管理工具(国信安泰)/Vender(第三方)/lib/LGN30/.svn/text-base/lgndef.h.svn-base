/*! \file lgndef.h 
 *  \brief 宏定义，基本数据类型定义
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNDEF_H__
#define __LGNDEF_H__

//#pragma once


// os type
#ifndef LGN_WINSYS
# ifdef WIN32
#  define LGN_WINSYS 1 //!< 1-WIN系统
# else
#  define LGN_WINSYS 0 //!< 0-非WIN系统
# endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// win32
#if (LGN_WINSYS)

#ifdef _LGN_NO_DEFAULT_LIBS
#define _LGN_NO_EXCEPTIONS //!< 不用默认库下，不支持异常
#endif

#ifndef LGN_SUPPORT_UNICODE
#define LGN_SUPPORT_UNICODE 1 //!< WIN下默认，支持UNICODE
#endif

#ifndef LGN_SUPPORT_MULTITHREAD
#define LGN_SUPPORT_MULTITHREAD 1 //!< WIN下默认，支持多线程
#endif

#ifndef LGN_SUPPORT_LITTLEENDIAN
#define LGN_SUPPORT_LITTLEENDIAN 1 //!< WIN、LINUX下默认，采用低地址存放最低有效字节
#endif


#ifdef ATLASSERT
#define LGNASSERT ATLASSERT
#else
#define LGNASSERT(exp) ((void)0)
#endif
#define LGNASSERT_VALID(x) __noop


#define LGN_CDECL __cdecl
#define LGN_INLINE inline
#if (_MSC_VER >= 1300)
#define LGN_NOINLINE __declspec( noinline )
#define LGN_SUPPORT_SELECTANY 1 //!< VC7后，在头文件里可以定义变量
#define LGN_HI_TEMPLATE 1 //!< VC7后，调用函数可以指定参数模板
#else
#define LGN_NOINLINE
#define LGN_SUPPORT_SELECTANY 0 //!< VC6，在头文件里不可以定义变量
#define LGN_HI_TEMPLATE 0 //!< VC6，调用函数不可以指定参数模板
#endif
#if (_MSC_VER >= 1000)
#define LGN_NORETURN __declspec( noreturn )
#else
#define LGN_NORETURN
#endif
#define LGN_SELECTANY __declspec(selectany)
#define LGN_NO_VTABLE __declspec(novtable)
#define LGN_DLL_EXPORT __declspec(dllexport)
#define LGN_DLL_IMPORT __declspec(dllimport)
#define LGN_INTERFACE interface
#define LGN_NOOP __noop

#define LGN_BR "\r\n"
#define LGN_BR_W L"\r\n"

// unix
#else // (!LGN_WINSYS)


#ifndef LGN_SUPPORT_UNICODE
#define LGN_SUPPORT_UNICODE 0
#endif

#ifndef LGN_SUPPORT_MULTITHREAD
#define LGN_SUPPORT_MULTITHREAD 0
#endif

#define LGNASSERT(exp) ((void)0)
#define LGNASSERT_VALID(x)
#define LGN_CDECL
#define LGN_INLINE inline
#define LGN_NOINLINE
#define LGN_SUPPORT_SELECTANY 0
#define LGN_HI_TEMPLATE 0
#define LGN_SELECTANY
#define LGN_DLL_EXPORT
#define LGN_DLL_IMPORT
#define LGN_INTERFACE struct
#define LGN_NOOP

#define LGN_BR "\n"

#ifndef TEXT
#define TEXT(quote) quote 
#endif

#ifndef _CPPUNWIND
#define _CPPUNWIND
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifndef IN
#define IN
#define OUT
#endif

#ifndef S_OK
#define S_OK				0x00000000
#define S_FALSE				0x00000001
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)
#define FAILED(Status) ((HRESULT)(Status)<0)
#endif


#endif // #if (LGN_WINSYS) else



////////////////////////////////////////////////////////////////////////////////////////////////////
// 基本数据类型
#ifndef LGN_BASETYPE_DECLARE
#define LGN_BASETYPE_DECLARE
#if (LGN_WINSYS)
	typedef DWORD				PID_T;
	typedef DWORD				TID_T;
#else  // unix
	typedef int					PID_T;
	typedef void *				TID_T;
	#define __int64 long long
	typedef unsigned int		UINT, UINT_PTR;
#if __LP64__
typedef signed int				HRESULT;//signed long与iokit里的定义冲突
typedef unsigned int		ULONG, ULONG_PTR;//unsigned long与iokit里的定义冲突
#else
typedef signed long				HRESULT;
typedef unsigned long		ULONG, ULONG_PTR;
#endif
	typedef char				CHAR, *LPSTR;
	typedef signed short		WCHAR, *LPWSTR;
	typedef const CHAR *		LPCSTR;
	typedef const WCHAR *		LPCWSTR;
	#ifdef UNICODE
	typedef WCHAR				TCHAR, *LPTSTR;
	#else
	typedef CHAR				TCHAR, *LPTSTR;
	#endif
	typedef const TCHAR *		LPCTSTR;
#endif
#endif // #ifndef LGN_BASETYPE_DECLARE

typedef signed char         INT8, *LPINT8;
typedef signed short        INT16, *LPINT16;
typedef signed int          INT32, *LPINT32;
typedef signed __int64      INT64, *LPINT64;
typedef unsigned char       UINT8, *LPUINT8;
typedef unsigned short      UINT16, *LPUINT16;
typedef unsigned int        UINT32, *LPUINT32;
typedef unsigned __int64    UINT64, *LPUINT64;

typedef void *				HANDLE;
typedef void *				LPVOID;
typedef const void *		LPCVOID;

typedef	unsigned char		BYTE, *LPBYTE;
typedef const BYTE *		LPCBYTE;
//


#if (LGN_SUPPORT_UNICODE)
#define LGN_TEXT(quote) L##quote //!< 定义了宏LGN_SUPPORT_UNICODE为真，UNICODE字符串
#else
#define LGN_TEXT(quote) quote //!< 未定义了宏LGN_SUPPORT_UNICODE为假，ANSI字符串
#endif


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////
// except
#if (!defined(_CPPUNWIND) && !defined(_LGN_NO_EXCEPTIONS))
#define _LGN_NO_EXCEPTIONS //!< 不支持C++异常
#endif

#ifndef _LGN_NO_EXCEPTIONS

#define _LGN__TRY __try
#define _LGN__EXCEPT(hr) __except(hr)

#ifndef LGNTRYALLOC
#ifdef _AFX
#define LGNTRYALLOC(x) try{x;} catch(CException* e){e->Delete();}
#else
#define LGNTRYALLOC(x) try{x;} catch(...){}
#endif	//__AFX
#endif

// If you define _LGNTRY before including this file, then
// you should define _LGNCATCH and _LGNRETHROW as well.
#ifndef _LGNTRY
#define _LGNTRY try
#ifdef _AFX
#define _LGNCATCH( e ) catch( CException* e )
#define _LGNGETEXCEPTIONSC( e ) e->IsKindOf(RUNTIME_CLASS(CMemoryException)) ? E_OUTOFMEMORY : (e->IsKindOf(RUNTIME_CLASS(COleException))? (((COleException *)e)->m_sc) : E_UNEXPECTED)
#else
#define _LGNCATCH( e ) catch( LGN::CLgnException e )
#define _LGNGETEXCEPTIONSC( e ) (HRESULT)(e)
#endif

#define _LGNCATCHALL() catch( ... )

#ifdef _AFX
#define _LGNDELETEEXCEPTION( e ) e->Delete();
#else
#define _LGNDELETEEXCEPTION( e ) e;
#endif

#define _LGNRETHROW throw
#endif	// _LGNTRY


#else //_LGN_NO_EXCEPTIONS

#define _LGN__TRY
#define _LGN__EXCEPT(hr) if(false)

#ifndef LGNTRYALLOC
#define LGNTRYALLOC(x) x;
#endif

// if _LGNTRY is defined before including this file then 
// _LGNCATCH and _LGNRETHROW should be defined as well.
#ifndef _LGNTRY
#define _LGNTRY
#define _LGNCATCH( e ) __pragma(warning(push)) __pragma(warning(disable: 4127)) if( false ) __pragma(warning(pop))
#define _LGNCATCHALL() __pragma(warning(push)) __pragma(warning(disable: 4127)) if( false ) __pragma(warning(pop))
#define _LGNGETEXCEPTIONSC(e) -1
#define _LGNDELETEEXCEPTION(e)
#define _LGNRETHROW
#endif	// _LGNTRY

#endif	//_LGN_NO_EXCEPTIONS


#ifndef LGNTRY
#define LGNTRY(x) LGNTRYALLOC(x)
#endif	//LGNTRY



////////////////////////////////////////////////////////////////////////////////////////////////////
// Master version numbers

#define _LGN     1      //!< LGN
#define _LGN_VER 0x0300 //!< LGN version 3.00


// error
#define E_LGN_FAIL						0xE010FFFF //!< LGN通常失败错误值
#define E_LGN_IO_EOF					0x80070026
#define E_LGN_INVALID_DATA				0x8007000D
#define E_LGN_NOT_SUPPORTED				0x80070032

#ifndef E_FAIL
#define E_FAIL							0x80004005
#define E_OUTOFMEMORY					0x8007000E
#define E_INVALIDARG					0x80070057
#endif


// 常量定义
#ifndef GMEM_ZEROINIT
#define GMEM_ZEROINIT					0x0040
#endif

#ifndef INFINITE
#define INFINITE						0xFFFFFFFF
#endif


#endif // #ifndef __LGNDEF_H__
