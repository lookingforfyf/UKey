/*! \file lgnwin.except.h 
 *  \brief WIN系统的异常处理
 *
 *  被lgnbase.h包含，工程不用直接包含本文件
 *
 *  主要四类异常：C异常、LGN异常、ATL异常、AFX异常
 *  <ul>
 *  	<li> 启用C异常：定义_LGN_NO_EXCEPTIONS
 *		<li> 启用LGN异常：默认方式，没有定义_LGN_NO_EXCEPTIONS，且没有包含atl、没有包含afx（MFC）
 *  	<li> 启用ATL异常：没有定义_LGN_NO_EXCEPTIONS，且包含atl、没有包含afx（MFC）
 *  	<li> 启用AFX异常：没有定义_LGN_NO_EXCEPTIONS，且包含afx（MFC）
 *  </ul>
 *  \author ljc
 *  \version 3.0.0
 *  \date    2010-2012
 *  \bug
 *  \warning
 */

#ifndef __LGNWIN_EXCEPT_H__
#define __LGNWIN_EXCEPT_H__

//#pragma once


/////////////////////////////////////////////////////////////////////////////
// Win32 libraries


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

	//! 抛出C异常（不能有返回）。内部调用<B>RaiseException</B>，具体参考MSDN 
	inline void LGN_NORETURN _LgnRaiseException( UINT32 dwExceptionCode, UINT32 dwExceptionFlags = EXCEPTION_NONCONTINUABLE )
	{
		::RaiseException( dwExceptionCode, dwExceptionFlags, 0, NULL );
	}


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __ATLEXCEPT_H__
	//! 类型重定义，使用ATL的异常类
	typedef ATL::CAtlException CLgnException;
#else
	//! LGN的异常类
	class CLgnException
	{
	public:
		CLgnException() throw() :
			m_hr( E_FAIL )
		{
		}

		CLgnException( HRESULT hr ) throw() :
			m_hr( hr )
		{
		}

		operator HRESULT() const throw()
		{
			return( m_hr );
		}

	public:
		HRESULT m_hr;
	};
#endif // #ifdef __ATLEXCEPT_H__


////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LGN_NO_EXCEPTIONS

// Throw a CAtlException with the given HRESULT
#if defined( LgnThrow ) || defined( _LGN_CUSTOM_THROW )  // You can define your own AtlThrow to throw a custom exception.
#ifdef _AFX
#error MFC projects must use default implementation of LgnThrow()
#endif
#else
	//! 抛出异常对象
	/*! 在使用AFX的工程里（MFC），则抛出AFX异常
	\param[in] hr 错误码（必须小于0）
	*/
	LGN_NOINLINE LGN_NORETURN inline void WINAPI LgnThrow( HRESULT hr )
	{
		//LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Emergency, TEXT("AtlThrow: hr = 0x%x\n"), hr );
#ifdef _AFX
		if( hr == E_OUTOFMEMORY )
		{
			AfxThrowMemoryException();
		}
		else
		{
			AfxThrowOleException( hr );
		}
#else
		throw CLgnException( hr );
#endif
	};
#endif

	//! 抛出异常对象（错误号为GetLastError）
	/*! \see LgnThrow( API::HResultFromError(GetLastError()) ) */
	LGN_NOINLINE LGN_NORETURN inline void WINAPI LgnThrowLastWin32()
	{
		LgnThrow( API::HResultFromError( ::GetLastError() ) );
	}

#else  // no exception handling

// Throw a CLgnException with the given HRESULT
#if !defined( LgnThrow ) && !defined( _LGN_CUSTOM_THROW )  // You can define your own LgnThrow

	LGN_NOINLINE inline void WINAPI LgnThrow( HRESULT hr )
	{
		//LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Emergency, TEXT("LgnThrow: hr = 0x%x\n"), hr );
		//LGNASSERT( false );
		UINT32 dwExceptionCode;
		switch(hr)
		{
		case E_OUTOFMEMORY:
			dwExceptionCode = STATUS_NO_MEMORY;
			break;
		default:
			dwExceptionCode = EXCEPTION_ILLEGAL_INSTRUCTION;
		}
		_LgnRaiseException(dwExceptionCode);
	}
#endif

	// Throw a CLgnException corresponding to the result of ::GetLastError
	LGN_NOINLINE inline void WINAPI LgnThrowLastWin32()
	{
		LgnThrow( API::HResultFromError( ::GetLastError() ) );
	}

#endif  // no exception handling


/*! \page lgnexception_example_page CLgnException例子
<pre>	%CLgnException类的使用例子</pre>
<pre>
\#include "lgnbase.h"

	// ...
	try
	{
		LGN::LgnThrow(-1);
	}
	catch(LGN::CLgnException e)
	{
		HRESULT hr = e;
	}
</pre>
*/

};  // namespace LGN

#endif  // __LGNWIN_EXCEPT_H__
