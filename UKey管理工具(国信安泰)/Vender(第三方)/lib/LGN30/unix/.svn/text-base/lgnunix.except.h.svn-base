#ifndef __LGNUNIX_EXCEPT_H__
#define __LGNUNIX_EXCEPT_H__


namespace LGN
{

/////////////////////////////////////////////////////////////////////////////
// Exception raise (for functions that cannot return an error code)

inline void _LgnRaiseException( UINT32 dwExceptionCode, UINT32 dwExceptionFlags = 0x01 )
{
	LGNASSERT(false);
}

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

#ifndef _LGN_NO_EXCEPTIONS

// Throw a CAtlException with the given HRESULT
#if defined( LgnThrow ) || defined( _LGN_CUSTOM_THROW )  // You can define your own AtlThrow to throw a custom exception.
#else
LGN_NOINLINE inline void LgnThrow( HRESULT hr )
{
	//LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Emergency, TEXT("AtlThrow: hr = 0x%x\n"), hr );
	throw CLgnException( hr );
};
#endif

#else  // no exception handling

// Throw a CLgnException with the given HRESULT
#if !defined( LgnThrow ) && !defined( _LGN_CUSTOM_THROW )  // You can define your own LgnThrow

LGN_NOINLINE inline void LgnThrow( HRESULT hr )
{
	//LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Emergency, TEXT("LgnThrow: hr = 0x%x\n"), hr );
	//LGNASSERT( false );
	_LgnRaiseException(hr);
}
#endif

#endif  // no exception handling

};  // namespace LGN

#endif  // __LGNUNIX_EXCEPT_H__
