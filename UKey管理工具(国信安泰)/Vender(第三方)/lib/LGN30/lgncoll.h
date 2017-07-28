/*! \file lgncoll.h 
*  \brief 集合模板集类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  集合模板类CLgnArray、CLgnList，CLgnMap，CSmartPtrArray，CSmartPtrList和CMapToSmartPtr；声明全线程安全智能对象引用列表模板类CSafeHandleList
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGNCOLL_H__
#define __LGNCOLL_H__


#if (LGN_WINSYS)
#include <new.h>
#else
#include <new>
#endif

#ifndef __LGN_BASE_H__
#error lgnwin.gsmem.h requires include lgnbase.h
#endif


namespace LGN
{
////////////////////////////////////////////////////////////////////////////////////////////////////

//! abstract iteration position
/*! 来源 atlcoll.h，同<B>ATL::__POSITION</B>，具体参考MSDN
*/
struct __POSITION
{
};
typedef __POSITION* POSITION;


#ifndef _LGN_PACKING
#define _LGN_PACKING 4
#endif

#ifndef UINT_MAX
#define UINT_MAX      0xffffffff    /* maximum unsigned int value */
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////

//! warning variable length structure
/*! 来源 atlcoll.h，同<B>ATL::CAtlPlex</B>，具体参考MSDN
*/
struct CLgnPlex
{
	CLgnPlex* pNext;
#if (_LGN_PACKING >= 8)
	UINT32 dwReserved[1];    // align on 8 byte boundary
#endif
	// BYTE data[maxNum*elementSize];

	void* data() { return this+1; }

	static CLgnPlex* Create(CLgnPlex*& head, size_t nMax, size_t cbElement);
			// like 'calloc' but no zero fill
			// may throw memory exceptions

	void FreeDataChain();       // free this one and links
};

inline CLgnPlex* CLgnPlex::Create( CLgnPlex*& pHead, size_t nMax, size_t nElementSize )
{
	CLgnPlex* pPlex;

	LGNASSERT( nMax > 0 );
	LGNASSERT( nElementSize > 0 );

	pPlex = static_cast< CLgnPlex* >( _lgnMemoryManager->Allocate( sizeof( CLgnPlex )+(nMax*nElementSize) ) );
	if( pPlex == NULL )
	{
		return( NULL );
	}

	pPlex->pNext = pHead;
	pHead = pPlex;

	return( pPlex );
}

inline void CLgnPlex::FreeDataChain()
{
	CLgnPlex* pPlex;

	pPlex = this;
	while( pPlex != NULL )
	{
		CLgnPlex* pNext;

		pNext = pPlex->pNext;
		_lgnMemoryManager->Free( pPlex );
		pPlex = pNext;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////

//! 来源 atlcoll.h，同<B>ATL::CElementTraitsBase</B>，具体参考MSDN
template< typename T >
class CElementTraitsBase
{
public:
	typedef const T& INARGTYPE;
	typedef T& OUTARGTYPE;

	static void CopyElements( T* pDest, const T* pSrc, size_t nElements )
	{
		for( size_t iElement = 0; iElement < nElements; iElement++ )
		{
			pDest[iElement] = pSrc[iElement];
		}
	}

	static void RelocateElements( T* pDest, T* pSrc, size_t nElements )
	{
		// A simple memmove works for nearly all types.
		// You'll have to override this for types that have pointers to their
		// own members.
		LGN::API::memmove( pDest, pSrc, nElements*sizeof( T ) );
	}
};

//! 来源 atlcoll.h，同<B>ATL::CDefaultHashTraits</B>，具体参考MSDN
template< typename T >
class CDefaultHashTraits
{
public:
	static ULONG Hash( const T& element ) throw()
	{
		return( ULONG( ULONG_PTR( element ) ) );
	}
};

//! 来源 atlcoll.h，同<B>ATL::CDefaultCompareTraits</B>，具体参考MSDN
template< typename T >
class CDefaultCompareTraits
{
public:
	static bool CompareElements( const T& element1, const T& element2 )
	{
		return( (element1 == element2) != 0 );  // != 0 to handle overloads of operator== that return BOOL instead of bool
	}

	static int CompareElementsOrdered( const T& element1, const T& element2 )
	{
		if( element1 < element2 )
		{
			return( -1 );
		}
		else if( element1 == element2 )
		{
			return( 0 );
		}
		else
		{
			LGNASSERT( element1 > element2 );
			return( 1 );
		}
	}
};

//! 来源 atlcoll.h，同<B>ATL::CDefaultElementTraits</B>，具体参考MSDN
template< typename T >
class CDefaultElementTraits :
	public CElementTraitsBase< T >,
	public CDefaultHashTraits< T >,
	public CDefaultCompareTraits< T >
{
};

//! 来源 atlcoll.h，同<B>ATL::CElementTraits</B>，具体参考MSDN
template< typename T >
class CElementTraits :
	public CDefaultElementTraits< T >
{
};

#if (LGN_WINSYS)
//! 来源 atlcoll.h，同<B>ATL::CElementTraits<GUID></B>，具体参考MSDN
template<>
class CElementTraits< GUID > :
	public CElementTraitsBase< GUID >
{
public:
	static ULONG Hash( INARGTYPE guid )
	{
		const UINT32* pdwData = reinterpret_cast< const UINT32* >( &guid );

		return( pdwData[0]^pdwData[1]^pdwData[2]^pdwData[3] );
	}

	static bool CompareElements( INARGTYPE element1, INARGTYPE element2 )
	{
#if (_MSC_VER >= 1300) // vc7
		return( (element1 == element2) != 0 );  // != 0 to handle overloads of operator== that return BOOL instead of bool
#else
		return (API::memcmp(&element1, &element2, sizeof(GUID)) == 0);
#endif
	}

	static int CompareElementsOrdered( INARGTYPE element1, INARGTYPE element2 )
	{
		const UINT32* pdwData1 = reinterpret_cast< const UINT32* >( &element1 );
		const UINT32* pdwData2 = reinterpret_cast< const UINT32* >( &element2 );

		for( int iDWORD = 3; iDWORD >= 0; iDWORD-- )
		{
			if( pdwData1[iDWORD] > pdwData2[iDWORD] )
			{
				return( 1 );
			}
			else if( pdwData1[iDWORD] < pdwData2[iDWORD] )
			{
				return( -1 );
			}
		}

		return( 0 );
	}
};
#endif // #if (LGN_WINSYS)

//! 来源 atlcoll.h，同<B>ATL::CSmartPtrElementTraits</B>，具体参考MSDN
template< typename T >
class CSmartPtrElementTraits :
	public CDefaultElementTraits< LGN::CSmartPtr< T > >
{
public:
	typedef LGN::CSmartPtr< T >& INARGTYPE;
	typedef T*& OUTARGTYPE;
};

//! 来源 atlcoll.h，同<B>ATL::CStringRefElementTraits</B>，具体参考MSDN
template< typename T >
class CStringRefElementTraits :
	public CElementTraitsBase< T >
{
public:
	typedef const T& INARGTYPE;
	typedef T& OUTARGTYPE;

	static ULONG Hash( INARGTYPE buff ) throw()
	{
		ULONG nHash = 0;
		for (int i=0; i<buff.GetLength(); i++)
		{
			nHash = (nHash<<5)+nHash+buff.GetAt(i);
		}
		return( nHash );
	}

	static bool CompareElements( INARGTYPE buff1, INARGTYPE buff2 ) throw()
	{
		return( buff1.Compare( buff2 ) == 0 );
	}

	static int CompareElementsOrdered( INARGTYPE buff1, INARGTYPE buff2 ) throw()
	{
		return( buff1.CompareNoCase( buff2 ) );
	}
};


#ifdef __LGNBUFFER_H__
//! LGN特有类LGN::CByteBuffer的使用集合类
template<>
class CElementTraits< LGN::CByteBuffer > :
	public CStringRefElementTraits< LGN::CByteBuffer >
{
};
#endif

#ifdef __LGNSTR_H__
//! LGN特有类LGN::CStringA的使用集合类
template<>
class CElementTraits< LGN::CStringA > :
	public CStringRefElementTraits< LGN::CStringA >
{
};

//! LGN特有类LGN::CStringW的使用集合类
template<>
class CElementTraits< LGN::CStringW > :
	public CStringRefElementTraits< LGN::CStringW >
{
};
#endif // #ifdef __LGNBUFFER_H__


//! 来源 atlcoll.h，同<B>ATL::CPrimitiveElementTraits</B>，具体参考MSDN
template< typename T >
class CPrimitiveElementTraits :
	public CDefaultElementTraits< T >
{
public:
	typedef T INARGTYPE;
	typedef T& OUTARGTYPE;
};

#define _DECLARE_PRIMITIVE_TRAITS( T ) \
	template<> \
	class CElementTraits< T > : \
		public CPrimitiveElementTraits< T > \
	{ \
	};

_DECLARE_PRIMITIVE_TRAITS( unsigned char )
_DECLARE_PRIMITIVE_TRAITS( unsigned short )
_DECLARE_PRIMITIVE_TRAITS( unsigned int )
_DECLARE_PRIMITIVE_TRAITS( unsigned long )
_DECLARE_PRIMITIVE_TRAITS( unsigned __int64 )
_DECLARE_PRIMITIVE_TRAITS( signed char )
_DECLARE_PRIMITIVE_TRAITS( char )
_DECLARE_PRIMITIVE_TRAITS( short )
_DECLARE_PRIMITIVE_TRAITS( int )
_DECLARE_PRIMITIVE_TRAITS( long )
_DECLARE_PRIMITIVE_TRAITS( __int64 )
_DECLARE_PRIMITIVE_TRAITS( float )
_DECLARE_PRIMITIVE_TRAITS( double )
_DECLARE_PRIMITIVE_TRAITS( bool )
#ifdef _NATIVE_WCHAR_T_DEFINED
_DECLARE_PRIMITIVE_TRAITS( wchar_t )
#endif
_DECLARE_PRIMITIVE_TRAITS( void* )


////////////////////////////////////////////////////////////////////////////////////////////////////

//! 来源 atlcoll.h，同<B>ATL::CAtlArray</B>，具体参考MSDN
template< typename E, class ETraits = CElementTraits< E > >
class CLgnArray
{
public:
	typedef typename ETraits::INARGTYPE INARGTYPE;
	typedef typename ETraits::OUTARGTYPE OUTARGTYPE;

public:
	CLgnArray() throw();

	size_t GetCount() const throw();
	bool IsEmpty() const throw();
	bool SetCount( size_t nNewSize, int nGrowBy = -1 );

	void FreeExtra() throw();
	void RemoveAll() throw();

	const E& GetAt( size_t iElement ) const;
	void SetAt( size_t iElement, INARGTYPE element );
	E& GetAt( size_t iElement );

	const E* GetData() const throw();
	E* GetData() throw();

	void SetAtGrow( size_t iElement, INARGTYPE element );
	// Add an empty element to the end of the array
	size_t Add();
	// Add an element to the end of the array
	size_t Add( INARGTYPE element );
	size_t Append( const CLgnArray< E, ETraits >& aSrc );
	void Copy( const CLgnArray< E, ETraits >& aSrc );

	const E& operator[]( size_t iElement ) const;
	E& operator[]( size_t iElement );

	void InsertAt( size_t iElement, INARGTYPE element, size_t nCount = 1 );
	void InsertArrayAt( size_t iStart, const CLgnArray< E, ETraits >* paNew );
	void RemoveAt( size_t iElement, size_t nCount = 1 );

private:
	bool GrowBuffer( size_t nNewSize );

// Implementation
private:
	E* m_pData;
	size_t m_nSize;
	size_t m_nMaxSize;
	int m_nGrowBy;

private:
	static void CallConstructors( E* pElements, size_t nElements );
	static void CallDestructors( E* pElements, size_t nElements ) throw();

public:
	~CLgnArray() throw();

private:
	// Private to prevent use
	CLgnArray( const CLgnArray& ) throw();
	CLgnArray& operator=( const CLgnArray& ) throw();
};


template< typename E, class ETraits >
inline size_t CLgnArray< E, ETraits >::GetCount() const throw()
{
	return( m_nSize );
}

template< typename E, class ETraits >
inline bool CLgnArray< E, ETraits >::IsEmpty() const throw()
{
	return( m_nSize == 0 );
}

template< typename E, class ETraits >
inline void CLgnArray< E, ETraits >::RemoveAll() throw()
{
	SetCount( 0, -1 );
}

template< typename E, class ETraits >
inline const E& CLgnArray< E, ETraits >::GetAt( size_t iElement ) const
{
	LGNASSERT( iElement < m_nSize );
	if(iElement >= m_nSize)
		LGN::LgnThrow(E_INVALIDARG);

	return( m_pData[iElement] );
}

template< typename E, class ETraits >
inline void CLgnArray< E, ETraits >::SetAt( size_t iElement, INARGTYPE element )
{
	LGNASSERT( iElement < m_nSize );
	if(iElement >= m_nSize)
		LGN::LgnThrow(E_INVALIDARG);

	m_pData[iElement] = element;
}

template< typename E, class ETraits >
inline E& CLgnArray< E, ETraits >::GetAt( size_t iElement )
{
	LGNASSERT( iElement < m_nSize );
	if(iElement >= m_nSize)
		LGN::LgnThrow(E_INVALIDARG);

	return( m_pData[iElement] );
}

template< typename E, class ETraits >
inline const E* CLgnArray< E, ETraits >::GetData() const throw()
{
	return( m_pData );
}

template< typename E, class ETraits >
inline E* CLgnArray< E, ETraits >::GetData() throw()
{
	return( m_pData );
}

template< typename E, class ETraits >
inline size_t CLgnArray< E, ETraits >::Add()
{
	size_t iElement;

	iElement = m_nSize;
	SetCount( m_nSize+1 );

	return( iElement );
}

#pragma push_macro("new")
#undef new

template< typename E, class ETraits >
inline size_t CLgnArray< E, ETraits >::Add( INARGTYPE element )
{
	size_t iElement;

	iElement = m_nSize;
	if( iElement >= m_nMaxSize )
	{
		bool bSuccess = GrowBuffer( iElement+1 );
		if( !bSuccess )
		{
			LGN::LgnThrow( E_OUTOFMEMORY );
		}
	}
	::new( m_pData+iElement ) E( element );
	m_nSize++;

	return( iElement );
}

#pragma pop_macro("new")

template< typename E, class ETraits >
inline const E& CLgnArray< E, ETraits >::operator[]( size_t iElement ) const
{
	LGNASSERT( iElement < m_nSize );
	if(iElement >= m_nSize)
		LGN::LgnThrow(E_INVALIDARG);

	return( m_pData[iElement] );
}

template< typename E, class ETraits >
inline E& CLgnArray< E, ETraits >::operator[]( size_t iElement ) 
{
	LGNASSERT( iElement < m_nSize );
	if(iElement >= m_nSize)
		LGN::LgnThrow(E_INVALIDARG);

	return( m_pData[iElement] );
}

template< typename E, class ETraits >
CLgnArray< E, ETraits >::CLgnArray()  throw():
	m_pData( NULL ),
	m_nSize( 0 ),
	m_nMaxSize( 0 ),
	m_nGrowBy( 0 )
{
}

template< typename E, class ETraits >
CLgnArray< E, ETraits >::~CLgnArray() throw()
{
	if( m_pData != NULL )
	{
		CallDestructors( m_pData, m_nSize );
		_lgnMemoryManager->Free( m_pData );
	}
}

template< typename E, class ETraits >
bool CLgnArray< E, ETraits >::GrowBuffer( size_t nNewSize )
{
	if( nNewSize > m_nMaxSize )
	{
		if( m_pData == NULL )
		{
			size_t nAllocSize =  size_t( m_nGrowBy ) > nNewSize ? size_t( m_nGrowBy ) : nNewSize ;
			m_pData = static_cast< E* >( _lgnMemoryManager->Allocate( nAllocSize*sizeof( E ) ) );
			if( m_pData == NULL )
			{
				return( false );
			}
			m_nMaxSize = nAllocSize;
		}
		else
		{
			// otherwise, grow array
			size_t nGrowBy = m_nGrowBy;
			if( nGrowBy == 0 )
			{
				// heuristically determine growth when nGrowBy == 0
				//  (this avoids heap fragmentation in many situations)
				nGrowBy = m_nSize/8;
				nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
			}
			size_t nNewMax;
			if( nNewSize < (m_nMaxSize+nGrowBy) )
				nNewMax = m_nMaxSize+nGrowBy;  // granularity
			else
				nNewMax = nNewSize;  // no slush

			LGNASSERT( nNewMax >= m_nMaxSize );  // no wrap around
#ifdef SIZE_T_MAX
			LGNASSERT( nNewMax <= SIZE_T_MAX/sizeof( E ) ); // no overflow
#endif
			E* pNewData = static_cast< E* >( _lgnMemoryManager->Allocate( nNewMax*sizeof( E ) ) );
			if( pNewData == NULL )
			{
				return false;
			}

			// copy new data from old
			ETraits::RelocateElements( pNewData, m_pData, m_nSize );

			// get rid of old stuff (note: no destructors called)
			_lgnMemoryManager->Free( m_pData );
			m_pData = pNewData;
			m_nMaxSize = nNewMax;
		}
	}

	return true;
}	

template< typename E, class ETraits >
bool CLgnArray< E, ETraits >::SetCount( size_t nNewSize, int nGrowBy )
{
	LGNASSERT_VALID(this);

	if( nGrowBy != -1 )
	{
		m_nGrowBy = nGrowBy;  // set new size
	}

	if( nNewSize == 0 )
	{
		// shrink to nothing
		if( m_pData != NULL )
		{
			CallDestructors( m_pData, m_nSize );
			_lgnMemoryManager->Free( m_pData );
			m_pData = NULL;
		}
		m_nSize = 0;
		m_nMaxSize = 0;
	}
	else if( nNewSize <= m_nMaxSize )
	{
		// it fits
		if( nNewSize > m_nSize )
		{
			// initialize the new elements
			CallConstructors( m_pData+m_nSize, nNewSize-m_nSize );
		}
		else if( m_nSize > nNewSize )
		{
			// destroy the old elements
			CallDestructors( m_pData+nNewSize, m_nSize-nNewSize );
		}
		m_nSize = nNewSize;
	}
	else
	{
		bool bSuccess;

		bSuccess = GrowBuffer( nNewSize );
		if( !bSuccess )
		{
			return( false );
		}

		// construct new elements
		LGNASSERT( nNewSize > m_nSize );
		CallConstructors( m_pData+m_nSize, nNewSize-m_nSize );

		m_nSize = nNewSize;
	}

	return true;
}

template< typename E, class ETraits >
size_t CLgnArray< E, ETraits >::Append( const CLgnArray< E, ETraits >& aSrc )
{
	LGNASSERT_VALID(this);
	LGNASSERT( this != &aSrc );   // cannot append to itself

	size_t nOldSize = m_nSize;
	SetCount( m_nSize+aSrc.m_nSize );
	ETraits::CopyElements( m_pData+nOldSize, aSrc.m_pData, aSrc.m_nSize );

	return( nOldSize );
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::Copy( const CLgnArray< E, ETraits >& aSrc )
{
	LGNASSERT_VALID(this);
	LGNASSERT( this != &aSrc );   // cannot append to itself

	SetCount( aSrc.m_nSize );
	ETraits::CopyElements( m_pData, aSrc.m_pData, aSrc.m_nSize );
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::FreeExtra() throw()
{
	LGNASSERT_VALID(this);

	if( m_nSize != m_nMaxSize )
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		LGNASSERT( m_nSize <= (SIZE_T_MAX/sizeof( E )) ); // no overflow
#endif
		E* pNewData = NULL;
		if( m_nSize != 0 )
		{
			pNewData = (E*)_lgnMemoryManager->Allocate( m_nSize*sizeof( E ) );
			if( pNewData == NULL )
			{
				return;
			}

			// copy new data from old
			ETraits::RelocateElements( pNewData, m_pData, m_nSize );
		}

		// get rid of old stuff (note: no destructors called)
		_lgnMemoryManager->Free( m_pData );
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::SetAtGrow( size_t iElement, INARGTYPE element )
{
	LGNASSERT_VALID(this);
	size_t nOldSize;

	nOldSize = m_nSize;
	if( iElement >= m_nSize )
		SetCount( iElement+1, -1 );
	_LGNTRY
	{
		m_pData[iElement] = element;
	}
	_LGNCATCHALL()
	{
		if( m_nSize != nOldSize )
		{
			SetCount( nOldSize, -1 );
		}
		_LGNRETHROW;
	}
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::InsertAt( size_t iElement, INARGTYPE element, size_t nElements /*=1*/)
{
	LGNASSERT_VALID(this);
	LGNASSERT( nElements > 0 );     // zero size not allowed

	if( iElement >= m_nSize )
	{
		// adding after the end of the array
		SetCount( iElement+nElements, -1 );   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		size_t nOldSize = m_nSize;
		SetCount( m_nSize+nElements, -1 );  // grow it to new size
		// destroy intial data before copying over it
		CallDestructors( m_pData+nOldSize, nElements );
		// shift old data up to fill gap
		ETraits::RelocateElements( m_pData+(iElement+nElements), m_pData+iElement,
			nOldSize-iElement );

		_LGNTRY
		{
			// re-init slots we copied from
			CallConstructors( m_pData+iElement, nElements );
		}
		_LGNCATCHALL()
		{
			ETraits::RelocateElements( m_pData+iElement, m_pData+(iElement+nElements),
				nOldSize-iElement );
			SetCount( nOldSize, -1 );
			_LGNRETHROW;
		}
	}

	// insert new value in the gap
	LGNASSERT( (iElement+nElements) <= m_nSize );
	for( size_t iNewElement = iElement; iNewElement < (iElement+nElements); iNewElement++ )
	{
		m_pData[iNewElement] = element;
	}
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::RemoveAt( size_t iElement, size_t nElements )
{
	LGNASSERT_VALID(this);
	LGNASSERT( (iElement+nElements) <= m_nSize );

	if( (iElement+nElements) > m_nSize )
		LGN::LgnThrow(E_INVALIDARG);		
		
	// just remove a range
	size_t nMoveCount = m_nSize-(iElement+nElements);
	CallDestructors( m_pData+iElement, nElements );
	if( nMoveCount > 0 )
	{
		ETraits::RelocateElements( m_pData+iElement, m_pData+(iElement+nElements),
			nMoveCount );
	}
	m_nSize -= nElements;
}

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::InsertArrayAt( size_t iStartElement, 
	const CLgnArray< E, ETraits >* paNew )
{
	LGNASSERT_VALID(this);
	LGNASSERT( paNew != NULL );
	LGNASSERT_VALID(paNew);

	if( paNew->GetCount() > 0 )
	{
		InsertAt( iStartElement, paNew->GetAt( 0 ), paNew->GetCount() );
		for( size_t iElement = 0; iElement < paNew->GetCount(); iElement++ )
			SetAt( iStartElement+iElement, paNew->GetAt( iElement ) );
	}
}


#pragma push_macro("new")
#undef new

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::CallConstructors( E* pElements, size_t nElements )
{
	size_t iElement;

	_LGNTRY
	{
		for( iElement = 0; iElement < nElements; iElement++ )
		{
			::new( pElements+iElement ) E;
		}
	}
	_LGNCATCHALL()
	{
		while( iElement > 0 )
		{
			iElement--;
			pElements[iElement].~E();
		}

		_LGNRETHROW;
	}
}

#pragma pop_macro("new")

template< typename E, class ETraits >
void CLgnArray< E, ETraits >::CallDestructors( E* pElements, size_t nElements ) throw()
{
	(void)pElements;

	for( size_t iElement = 0; iElement < nElements; iElement++ )
	{
		pElements[iElement].~E();
	}
}

//! 来源 atlcoll.h，同<B>ATL::CSmartPtrArray</B>，具体参考MSDN
template< typename E >
class CSmartPtrArray :
	public CLgnArray< LGN::CSmartPtr< E >, CSmartPtrElementTraits< E > >
{
public:
	CSmartPtrArray() throw()
	{
	}

private:
	// Private to prevent use
	CSmartPtrArray( const CSmartPtrArray& ) throw();
	CSmartPtrArray& operator=( const CSmartPtrArray& ) throw();
};


////////////////////////////////////////////////////////////////////////////////////////////////////

//! 来源 atlcoll.h，同<B>ATL::CAtlList</B>，具体参考MSDN
template< typename E, class ETraits = CElementTraits< E > >
class CLgnList
{
public:
	typedef typename ETraits::INARGTYPE INARGTYPE;

private:
	class CNode :
		public __POSITION
	{
	public:
		CNode()
		{
		}
		CNode( INARGTYPE element ) :
			m_element( element )
		{
		}
		~CNode() throw()
		{
		}

	public:
		CNode* m_pNext;
		CNode* m_pPrev;
		E m_element;

	private:
		CNode( const CNode& ) throw();
	};

public:
	CLgnList( UINT nBlockSize = 10 ) throw();

	size_t GetCount() const throw();
	bool IsEmpty() const throw();

	E& GetHead() throw();
	const E& GetHead() const throw();
	E& GetTail() throw();
	const E& GetTail() const throw();

	E RemoveHead();
	E RemoveTail();
	void RemoveHeadNoReturn() throw();
	void RemoveTailNoReturn() throw();

	POSITION AddHead();
	POSITION AddHead( INARGTYPE element );
	void AddHeadList( const CLgnList< E, ETraits >* plNew );
	POSITION AddTail();
	POSITION AddTail( INARGTYPE element );
	void AddTailList( const CLgnList< E, ETraits >* plNew );

	void RemoveAll() throw();

	POSITION GetHeadPosition() const throw();
	POSITION GetTailPosition() const throw();
	E& GetNext( POSITION& pos ) throw();
	const E& GetNext( POSITION& pos ) const throw();
	E& GetPrev( POSITION& pos ) throw();
	const E& GetPrev( POSITION& pos ) const throw();

	E& GetAt( POSITION pos ) throw();
	const E& GetAt( POSITION pos ) const throw();
	void SetAt( POSITION pos, INARGTYPE element );
	void RemoveAt( POSITION pos ) throw();

	POSITION InsertBefore( POSITION pos, INARGTYPE element );
	POSITION InsertAfter( POSITION pos, INARGTYPE element );

	POSITION Find( INARGTYPE element, POSITION posStartAfter = NULL ) const throw();
	POSITION FindIndex( size_t iElement ) const throw();

	void MoveToHead( POSITION pos ) throw();
	void MoveToTail( POSITION pos ) throw();
	void SwapElements( POSITION pos1, POSITION pos2 ) throw();

// Implementation
private:
	CNode* m_pHead;
	CNode* m_pTail;
	size_t m_nElements;
	CLgnPlex* m_pBlocks;
	CNode* m_pFree;
	UINT m_nBlockSize;

private:
	void GetFreeNode();
	CNode* NewNode( CNode* pPrev, CNode* pNext );
	CNode* NewNode( INARGTYPE element, CNode* pPrev, CNode* pNext );
	void FreeNode( CNode* pNode ) throw();

public:
	~CLgnList() throw();

private:
	// Private to prevent use
	CLgnList( const CLgnList& ) throw();
	CLgnList& operator=( const CLgnList& ) throw();
};

template< typename E, class ETraits >
inline size_t CLgnList< E, ETraits >::GetCount() const throw()
{
	return( m_nElements );
}

template< typename E, class ETraits >
inline bool CLgnList< E, ETraits >::IsEmpty() const throw()
{
	return( m_nElements == 0 );
}

template< typename E, class ETraits >
inline E& CLgnList< E, ETraits >::GetHead() throw()
{
	LGNASSERT( m_pHead != NULL );
	return( m_pHead->m_element );
}

template< typename E, class ETraits >
inline const E& CLgnList< E, ETraits >::GetHead() const throw()
{
	LGNASSERT( m_pHead != NULL );
	return( m_pHead->m_element );
}

template< typename E, class ETraits >
inline E& CLgnList< E, ETraits >::GetTail() throw()
{
	LGNASSERT( m_pTail != NULL );
	return( m_pTail->m_element );
}

template< typename E, class ETraits >
inline const E& CLgnList< E, ETraits >::GetTail() const throw()
{
	LGNASSERT( m_pTail != NULL );
	return( m_pTail->m_element );
}

template< typename E, class ETraits >
inline POSITION CLgnList< E, ETraits >::GetHeadPosition() const throw()
{
	return( POSITION( m_pHead ) );
}

template< typename E, class ETraits >
inline POSITION CLgnList< E, ETraits >::GetTailPosition() const throw()
{
	return( POSITION( m_pTail ) );
}

template< typename E, class ETraits >
inline E& CLgnList< E, ETraits >::GetNext( POSITION& pos ) throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;
	pos = POSITION( pNode->m_pNext );

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline const E& CLgnList< E, ETraits >::GetNext( POSITION& pos ) const throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;
	pos = POSITION( pNode->m_pNext );

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline E& CLgnList< E, ETraits >::GetPrev( POSITION& pos ) throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;
	pos = POSITION( pNode->m_pPrev );

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline const E& CLgnList< E, ETraits >::GetPrev( POSITION& pos ) const throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;
	pos = POSITION( pNode->m_pPrev );

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline E& CLgnList< E, ETraits >::GetAt( POSITION pos ) throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline const E& CLgnList< E, ETraits >::GetAt( POSITION pos ) const throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;

	return( pNode->m_element );
}

template< typename E, class ETraits >
inline void CLgnList< E, ETraits >::SetAt( POSITION pos, INARGTYPE element )
{
	CNode* pNode;

	LGNASSERT( pos != NULL );
	pNode = (CNode*)pos;
	pNode->m_element = element;
}

template< typename E, class ETraits >
CLgnList< E, ETraits >::CLgnList( UINT nBlockSize ) throw() :
	m_nElements( 0 ),
	m_pHead( NULL ),
	m_pTail( NULL ),
	m_nBlockSize( nBlockSize ),
	m_pBlocks( NULL ),
	m_pFree( NULL )
{
	LGNASSERT( nBlockSize > 0 );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::RemoveAll() throw()
{
	while( m_nElements > 0 )
	{
		CNode* pKill;

		pKill = m_pHead;
		LGNASSERT( pKill != NULL );
		m_pHead = m_pHead->m_pNext;
		FreeNode( pKill );
	}
	LGNASSERT( m_nElements == 0 );
	m_pHead = NULL;
	m_pTail = NULL;
	m_pFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template< typename E, class ETraits >
CLgnList< E, ETraits >::~CLgnList() throw()
{
	RemoveAll();
	LGNASSERT( m_nElements == 0 );
}

#pragma push_macro("new")
#undef new

template< typename E, class ETraits >
void CLgnList< E, ETraits >::GetFreeNode()
{
	if( m_pFree == NULL )
	{
		CLgnPlex* pPlex;
		CNode* pNode;

		pPlex = CLgnPlex::Create( m_pBlocks, m_nBlockSize, sizeof( CNode ) );
		if( pPlex == NULL )
		{
			LGN::LgnThrow( E_OUTOFMEMORY );
		}
		pNode = (CNode*)pPlex->data();
		pNode += m_nBlockSize-1;
		for( int iBlock = m_nBlockSize-1; iBlock >= 0; iBlock-- )
		{
			pNode->m_pNext = m_pFree;
			m_pFree = pNode;
			pNode--;
		}
	}
	LGNASSERT( m_pFree != NULL );
}

template< typename E, class ETraits >
typename CLgnList< E, ETraits >::CNode* CLgnList< E, ETraits >::NewNode( CNode* pPrev, CNode* pNext )
{
	GetFreeNode();

	CNode* pNewNode = m_pFree;
	CNode* pNextFree = m_pFree->m_pNext;

	::new( pNewNode ) CNode;

	m_pFree = pNextFree;
	pNewNode->m_pPrev = pPrev;
	pNewNode->m_pNext = pNext;
	m_nElements++;
	LGNASSERT( m_nElements > 0 );

	return( pNewNode );
}

template< typename E, class ETraits >
typename CLgnList< E, ETraits >::CNode* CLgnList< E, ETraits >::NewNode( INARGTYPE element, CNode* pPrev, 
	CNode* pNext )
{
	GetFreeNode();

	CNode* pNewNode = m_pFree;
	CNode* pNextFree = m_pFree->m_pNext;

	::new( pNewNode ) CNode( element );

	m_pFree = pNextFree;
	pNewNode->m_pPrev = pPrev;
	pNewNode->m_pNext = pNext;
	m_nElements++;
	LGNASSERT( m_nElements > 0 );

	return( pNewNode );
}

#pragma pop_macro("new")

template< typename E, class ETraits >
void CLgnList< E, ETraits >::FreeNode( CNode* pNode ) throw()
{
	pNode->~CNode();
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
	LGNASSERT( m_nElements > 0 );
	m_nElements--;
	if( m_nElements == 0 )
	{
		RemoveAll();
	}
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::AddHead()
{
	CNode* pNode = NewNode( NULL, m_pHead );
	if( m_pHead != NULL )
	{
		m_pHead->m_pPrev = pNode;
	}
	else
	{
		m_pTail = pNode;
	}
	m_pHead = pNode;

	return( POSITION( pNode ) );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::AddHead( INARGTYPE element )
{
	CNode* pNode;

	pNode = NewNode( element, NULL, m_pHead );

	if( m_pHead != NULL )
	{
		m_pHead->m_pPrev = pNode;
	}
	else
	{
		m_pTail = pNode;
	}
	m_pHead = pNode;

	return( POSITION( pNode ) );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::AddTail()
{
	CNode* pNode = NewNode( m_pTail, NULL );
	if( m_pTail != NULL )
	{
		m_pTail->m_pNext = pNode;
	}
	else
	{
		m_pHead = pNode;
	}
	m_pTail = pNode;

	return( POSITION( pNode ) );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::AddTail( INARGTYPE element )
{
	CNode* pNode;

	pNode = NewNode( element, m_pTail, NULL );

	if( m_pTail != NULL )
	{
		m_pTail->m_pNext = pNode;
	}
	else
	{
		m_pHead = pNode;
	}
	m_pTail = pNode;

	return( POSITION( pNode ) );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::AddHeadList( const CLgnList< E, ETraits >* plNew )
{
	POSITION pos;

	LGNASSERT( plNew != NULL );

	pos = plNew->GetTailPosition();
	while( pos != NULL )
	{
		INARGTYPE element = plNew->GetPrev( pos );
		AddHead( element );
	}
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::AddTailList( const CLgnList< E, ETraits >* plNew )
{
	POSITION pos;

	LGNASSERT( plNew != NULL );

	pos = plNew->GetHeadPosition();
	while( pos != NULL )
	{
		INARGTYPE element = plNew->GetNext( pos );
		AddTail( element );
	}
}

template< typename E, class ETraits >
E CLgnList< E, ETraits >::RemoveHead()
{
	CNode* pNode;

	LGNASSERT( m_pHead != NULL );

	pNode = m_pHead;
	E element( pNode->m_element );

	m_pHead = pNode->m_pNext;
	if( m_pHead != NULL )
	{
		m_pHead->m_pPrev = NULL;
	}
	else
	{
		m_pTail = NULL;
	}
	FreeNode( pNode );

	return( element );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::RemoveHeadNoReturn() throw()
{
	LGNASSERT( m_pHead != NULL );

	CNode* pNode = m_pHead;
	m_pHead = pNode->m_pNext;
	if( m_pHead != NULL )
	{
		m_pHead->m_pPrev = NULL;
	}
	else
	{
		m_pTail = NULL;
	}
	FreeNode( pNode );
}

template< typename E, class ETraits >
E CLgnList< E, ETraits >::RemoveTail()
{
	CNode* pNode;

	LGNASSERT( m_pTail != NULL );

	pNode = m_pTail;
	E element( pNode->m_element );

	m_pTail = pNode->m_pPrev;
	if( m_pTail != NULL )
	{
		m_pTail->m_pNext = NULL;
	}
	else
	{
		m_pHead = NULL;
	}
	FreeNode( pNode );

	return( element );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::RemoveTailNoReturn() throw()
{
	LGNASSERT( m_pTail != NULL );

	CNode* pNode = m_pTail;

	m_pTail = pNode->m_pPrev;
	if( m_pTail != NULL )
	{
		m_pTail->m_pNext = NULL;
	}
	else
	{
		m_pHead = NULL;
	}
	FreeNode( pNode );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::InsertBefore( POSITION pos, INARGTYPE element )
{
	LGNASSERT_VALID(this);

	if( pos == NULL )
		return AddHead( element ); // insert before nothing -> head of the list

	// Insert it before position
	CNode* pOldNode = (CNode*)pos;
	CNode* pNewNode = NewNode( element, pOldNode->m_pPrev, pOldNode );

	if( pOldNode->m_pPrev != NULL )
	{
		//LGNASSERT(AtlIsValidAddress(pOldNode->m_pPrev, sizeof(CNode)));
		pOldNode->m_pPrev->m_pNext = pNewNode;
	}
	else
	{
		LGNASSERT( pOldNode == m_pHead );
		m_pHead = pNewNode;
	}
	pOldNode->m_pPrev = pNewNode;

	return( POSITION( pNewNode ) );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::InsertAfter( POSITION pos, INARGTYPE element )
{
	LGNASSERT_VALID(this);

	if( pos == NULL )
		return AddTail( element ); // insert after nothing -> tail of the list

	// Insert it after position
	CNode* pOldNode = (CNode*)pos;
	CNode* pNewNode = NewNode( element, pOldNode, pOldNode->m_pNext );

	if( pOldNode->m_pNext != NULL )
	{
		//LGNASSERT(AtlIsValidAddress(pOldNode->m_pNext, sizeof(CNode)));
		pOldNode->m_pNext->m_pPrev = pNewNode;
	}
	else
	{
		LGNASSERT( pOldNode == m_pTail );
		m_pTail = pNewNode;
	}
	pOldNode->m_pNext = pNewNode;

	return( POSITION( pNewNode ) );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::RemoveAt( POSITION pos ) throw()
{
	LGNASSERT_VALID(this);

	CNode* pOldNode = (CNode*)pos;
	//LGNASSERT(AtlIsValidAddress(pOldNode, sizeof(CNode)));

	// remove pOldNode from list
	if( pOldNode == m_pHead )
	{
		m_pHead = pOldNode->m_pNext;
	}
	else
	{
		//LGNASSERT(AtlIsValidAddress(pOldNode->m_pPrev, sizeof(CNode)));
		pOldNode->m_pPrev->m_pNext = pOldNode->m_pNext;
	}
	if( pOldNode == m_pTail )
	{
		m_pTail = pOldNode->m_pPrev;
	}
	else
	{
		//LGNASSERT(AtlIsValidAddress(pOldNode->m_pNext, sizeof(CNode)));
		pOldNode->m_pNext->m_pPrev = pOldNode->m_pPrev;
	}
	FreeNode( pOldNode );
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::FindIndex( size_t iElement ) const throw()
{
	LGNASSERT_VALID(this);

	if( iElement >= m_nElements )
		return NULL;  // went too far

	if(m_pHead == NULL)
		return NULL;
		
	CNode* pNode = m_pHead;
	for( size_t iSearch = 0; iSearch < iElement; iSearch++ )
	{
		pNode = pNode->m_pNext;
	}

	return( POSITION( pNode ) );
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::MoveToHead( POSITION pos ) throw()
{
	LGNASSERT( pos != NULL );

	CNode* pNode = static_cast< CNode* >( pos );
	if( pNode == m_pHead )
	{
		// Already at the head
		return;
	}

	if( pNode->m_pNext == NULL )
	{
		LGNASSERT( pNode == m_pTail );
		m_pTail = pNode->m_pPrev;
	}
	else
	{
		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}
	LGNASSERT( pNode->m_pPrev != NULL );  // This node can't be the head, since we already checked that case
	pNode->m_pPrev->m_pNext = pNode->m_pNext;

	m_pHead->m_pPrev = pNode;
	pNode->m_pNext = m_pHead;
	pNode->m_pPrev = NULL;
	m_pHead = pNode;
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::MoveToTail( POSITION pos ) throw()
{
	LGNASSERT( pos != NULL );

	CNode* pNode = static_cast< CNode* >( pos );
	if( pNode == m_pTail )
	{
		// Already at the tail
		return;
	}

	if( pNode->m_pPrev == NULL )
	{
		LGNASSERT( pNode == m_pHead );
		m_pHead = pNode->m_pNext;
	}
	else
	{
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}
	LGNASSERT( pNode->m_pNext != NULL );  // This node can't be the tail, since we already checked that case
	pNode->m_pNext->m_pPrev = pNode->m_pPrev;

	m_pTail->m_pNext = pNode;
	pNode->m_pPrev = m_pTail;
	pNode->m_pNext = NULL;
	m_pTail = pNode;
}

template< typename E, class ETraits >
void CLgnList< E, ETraits >::SwapElements( POSITION pos1, POSITION pos2 ) throw()
{
	LGNASSERT( pos1 != NULL );
	LGNASSERT( pos2 != NULL );

	if( pos1 == pos2 )
	{
		// Nothing to do
		return;
	}

	CNode* pNode1 = static_cast< CNode* >( pos1 );
	CNode* pNode2 = static_cast< CNode* >( pos2 );
	if( pNode2->m_pNext == pNode1 )
	{
		// Swap pNode2 and pNode1 so that the next case works
		CNode* pNodeTemp = pNode1;
		pNode1 = pNode2;
		pNode2 = pNodeTemp;
	}
	if( pNode1->m_pNext == pNode2 )
	{
		// Node1 and Node2 are adjacent
		pNode2->m_pPrev = pNode1->m_pPrev;
		if( pNode1->m_pPrev != NULL )
		{
			pNode1->m_pPrev->m_pNext = pNode2;
		}
		else
		{
			LGNASSERT( m_pHead == pNode1 );
			m_pHead = pNode2;
		}
		pNode1->m_pNext = pNode2->m_pNext;
		if( pNode2->m_pNext != NULL )
		{
			pNode2->m_pNext->m_pPrev = pNode1;
		}
		else
		{
			LGNASSERT( m_pTail == pNode2 );
			m_pTail = pNode1;
		}
		pNode2->m_pNext = pNode1;
		pNode1->m_pPrev = pNode2;
	}
	else
	{
		// The two nodes are not adjacent
		CNode* pNodeTemp;

		pNodeTemp = pNode1->m_pPrev;
		pNode1->m_pPrev = pNode2->m_pPrev;
		pNode2->m_pPrev = pNodeTemp;

		pNodeTemp = pNode1->m_pNext;
		pNode1->m_pNext = pNode2->m_pNext;
		pNode2->m_pNext = pNodeTemp;

		if( pNode1->m_pNext != NULL )
		{
			pNode1->m_pNext->m_pPrev = pNode1;
		}
		else
		{
			LGNASSERT( m_pTail == pNode2 );
			m_pTail = pNode1;
		}
		if( pNode1->m_pPrev != NULL )
		{
			pNode1->m_pPrev->m_pNext = pNode1;
		}
		else
		{
			LGNASSERT( m_pHead == pNode2 );
			m_pHead = pNode1;
		}
		if( pNode2->m_pNext != NULL )
		{
			pNode2->m_pNext->m_pPrev = pNode2;
		}
		else
		{
			LGNASSERT( m_pTail == pNode1 );
			m_pTail = pNode2;
		}
		if( pNode2->m_pPrev != NULL )
		{
			pNode2->m_pPrev->m_pNext = pNode2;
		}
		else
		{
			LGNASSERT( m_pHead == pNode1 );
			m_pHead = pNode2;
		}
	}
}

template< typename E, class ETraits >
POSITION CLgnList< E, ETraits >::Find( INARGTYPE element, POSITION posStartAfter ) const throw()
{
	LGNASSERT_VALID(this);

	CNode* pNode = (CNode*)posStartAfter;
	if( pNode == NULL )
	{
		pNode = m_pHead;  // start at head
	}
	else
	{
		//LGNASSERT(AtlIsValidAddress(pNode, sizeof(CNode)));
		pNode = pNode->m_pNext;  // start after the one specified
	}

	for( ; pNode != NULL; pNode = pNode->m_pNext )
	{
		if( ETraits::CompareElements( pNode->m_element, element ) )
			return( POSITION( pNode ) );
	}

	return( NULL );
}

//! 来源 atlcoll.h，同<B>ATL::CSmartPtrList</B>，具体参考MSDN
template< typename E >
class CSmartPtrList :
	public CLgnList< LGN::CSmartPtr< E >, CSmartPtrElementTraits< E > >
{
public:
	CSmartPtrList( UINT nBlockSize = 10 ) throw() :
		CLgnList< LGN::CSmartPtr< E >, CSmartPtrElementTraits< E > >( nBlockSize )
	{
	}

private:
	// Private to prevent use
	CSmartPtrList( const CSmartPtrList& ) throw();
	CSmartPtrList& operator=( const CSmartPtrList& ) throw();
};


////////////////////////////////////////////////////////////////////////////////////////////////////

//! 来源 atlcoll.h，同<B>ATL::CAtlMap</B>，具体参考MSDN
template< typename K, typename V, class KTraits = CElementTraits< K >, class VTraits = CElementTraits< V > >
class CLgnMap
{
public:
	typedef typename KTraits::INARGTYPE KINARGTYPE;
	typedef typename KTraits::OUTARGTYPE KOUTARGTYPE;
	typedef typename VTraits::INARGTYPE VINARGTYPE;
	typedef typename VTraits::OUTARGTYPE VOUTARGTYPE;

	//! KEY-VALUE
	class CPair :
		public __POSITION
	{
	protected:
		CPair( KINARGTYPE key ) :
			m_key( key )
		{
		}

	public:
		const K m_key;
		V m_value;
	};

private:
	class CNode :
		public CPair
	{
	public:
		CNode( KINARGTYPE key, UINT nHash ) :
			CPair( key ),
			m_nHash( nHash )
		{
		}

	public:
		UINT GetHash() const throw()
		{
			return( m_nHash );
		}

	public:
		CNode* m_pNext;
		UINT m_nHash;
	};

public:
	CLgnMap( UINT nBins = 17, float fOptimalLoad = 0.75f, 
		float fLoThreshold = 0.25f, float fHiThreshold = 2.25f, UINT nBlockSize = 10 ) throw();

	size_t GetCount() const throw();
	bool IsEmpty() const throw();

	bool Lookup( KINARGTYPE key, VOUTARGTYPE value ) const;
	const CPair* Lookup( KINARGTYPE key ) const throw();
	CPair* Lookup( KINARGTYPE key ) throw();
	V& operator[]( KINARGTYPE key ) throw();

	POSITION SetAt( KINARGTYPE key, VINARGTYPE value );
	void SetValueAt( POSITION pos, VINARGTYPE value );

	bool RemoveKey( KINARGTYPE key ) throw();
	void RemoveAll() throw();
	void RemoveAtPos( POSITION pos ) throw();

	POSITION GetStartPosition() const throw();
	void GetNextAssoc( POSITION& pos, KOUTARGTYPE key, VOUTARGTYPE value ) const;
	const CPair* GetNext( POSITION& pos ) const throw();
	CPair* GetNext( POSITION& pos ) throw();
	const K& GetNextKey( POSITION& pos ) const throw();
	const V& GetNextValue( POSITION& pos ) const throw();
	V& GetNextValue( POSITION& pos ) throw();
	void GetAt( POSITION pos, KOUTARGTYPE key, VOUTARGTYPE value ) const;
	CPair* GetAt( POSITION pos ) throw();
	const CPair* GetAt( POSITION pos ) const throw();
	const K& GetKeyAt( POSITION pos ) const throw();
	const V& GetValueAt( POSITION pos ) const throw();
	V& GetValueAt( POSITION pos ) throw();

	UINT GetHashTableSize() const throw();
	bool InitHashTable( UINT nBins, bool bAllocNow = true );
	void EnableAutoRehash() throw();
	void DisableAutoRehash() throw();
	void Rehash( UINT nBins = 0 );
	void SetOptimalLoad( float fOptimalLoad, float fLoThreshold, float fHiThreshold, 
		bool bRehashNow = false );

// Implementation
private:
	CNode** m_ppBins;
	size_t m_nElements;
	UINT m_nBins;
	float m_fOptimalLoad;
	float m_fLoThreshold;
	float m_fHiThreshold;
	size_t m_nHiRehashThreshold;
	size_t m_nLoRehashThreshold;
	ULONG m_nLockCount;
	UINT m_nBlockSize;
	CLgnPlex* m_pBlocks;
	CNode* m_pFree;

private:
	bool IsLocked() const throw();
	UINT PickSize( size_t nElements ) const throw();
	CNode* NewNode( KINARGTYPE key, UINT iBin, UINT nHash );
	void FreeNode( CNode* pNode ) throw();
	void FreePlexes() throw();
	CNode* GetNode( KINARGTYPE key, UINT& iBin, UINT& nHash, CNode*& pPrev ) const throw();
	CNode* CreateNode( KINARGTYPE key, UINT iBin, UINT nHash );
	void RemoveNode( CNode* pNode, CNode* pPrev ) throw();
	CNode* FindNextNode( CNode* pNode ) const throw();
	void UpdateRehashThresholds() throw();

public:
	~CLgnMap() throw();

private:
	// Private to prevent use
	CLgnMap( const CLgnMap& ) throw();
	CLgnMap& operator=( const CLgnMap& ) throw();
};

template< typename K, typename V, class KTraits, class VTraits >
inline size_t CLgnMap< K, V, KTraits, VTraits >::GetCount() const throw()
{
	return( m_nElements );
}

template< typename K, typename V, class KTraits, class VTraits >
inline bool CLgnMap< K, V, KTraits, VTraits >::IsEmpty() const throw()
{
	return( m_nElements == 0 );
}

template< typename K, typename V, class KTraits, class VTraits >
inline V& CLgnMap< K, V, KTraits, VTraits >::operator[]( KINARGTYPE key ) throw()
{
	CNode* pNode;
	UINT iBin;
	UINT nHash;
	CNode* pPrev;

	pNode = GetNode( key, iBin, nHash, pPrev );
	if( pNode == NULL )
	{
		pNode = CreateNode( key, iBin, nHash );
	}

	return( pNode->m_value );
}

template< typename K, typename V, class KTraits, class VTraits >
inline UINT CLgnMap< K, V, KTraits, VTraits >::GetHashTableSize() const throw()
{
	return( m_nBins );
}

template< typename K, typename V, class KTraits, class VTraits >
inline void CLgnMap< K, V, KTraits, VTraits >::GetAt( POSITION pos, KOUTARGTYPE key, VOUTARGTYPE value ) const
{
	CNode* pNode;

	LGNASSERT( pos != NULL );

	pNode = static_cast< CNode* >( pos );
	key = pNode->m_key;
	value = pNode->m_value;
}

template< typename K, typename V, class KTraits, class VTraits >
inline typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::GetAt( POSITION pos ) throw()
{
	LGNASSERT( pos != NULL );

	return( static_cast< CPair* >( pos ) );
}

template< typename K, typename V, class KTraits, class VTraits >
inline const typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::GetAt( POSITION pos ) const throw()
{
	LGNASSERT( pos != NULL );

	return( static_cast< const CPair* >( pos ) );
}

template< typename K, typename V, class KTraits, class VTraits >
inline const K& CLgnMap< K, V, KTraits, VTraits >::GetKeyAt( POSITION pos ) const throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;

	return( pNode->m_key );
}

template< typename K, typename V, class KTraits, class VTraits >
inline const V& CLgnMap< K, V, KTraits, VTraits >::GetValueAt( POSITION pos ) const throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;

	return( pNode->m_value );
}

template< typename K, typename V, class KTraits, class VTraits >
inline V& CLgnMap< K, V, KTraits, VTraits >::GetValueAt( POSITION pos ) throw()
{
	CNode* pNode;

	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;

	return( pNode->m_value );
}

template< typename K, typename V, class KTraits, class VTraits >
inline void CLgnMap< K, V, KTraits, VTraits >::DisableAutoRehash() throw()
{
	m_nLockCount++;
}

template< typename K, typename V, class KTraits, class VTraits >
inline void CLgnMap< K, V, KTraits, VTraits >::EnableAutoRehash() throw()
{
	LGNASSERT( m_nLockCount > 0 );
	m_nLockCount--;
}

template< typename K, typename V, class KTraits, class VTraits >
inline bool CLgnMap< K, V, KTraits, VTraits >::IsLocked() const throw()
{
	return( m_nLockCount != 0 );
}

template< typename K, typename V, class KTraits, class VTraits >
UINT CLgnMap< K, V, KTraits, VTraits >::PickSize( size_t nElements ) const throw()
{
	// List of primes such that s_anPrimes[i] is the smallest prime greater than 2^(5+i/3)
	static const UINT s_anPrimes[] =
	{
		17, 23, 29, 37, 41, 53, 67, 83, 103, 131, 163, 211, 257, 331, 409, 521, 647, 821, 
		1031, 1291, 1627, 2053, 2591, 3251, 4099, 5167, 6521, 8209, 10331, 
		13007, 16411, 20663, 26017, 32771, 41299, 52021, 65537, 82571, 104033, 
		131101, 165161, 208067, 262147, 330287, 416147, 524309, 660563, 
		832291, 1048583, 1321139, 1664543, 2097169, 2642257, 3329023, 4194319, 
		5284493, 6658049, 8388617, 10568993, 13316089, UINT_MAX
	};

	size_t nBins = (size_t)(nElements/m_fOptimalLoad);
	UINT nBinsEstimate = UINT(  UINT_MAX < nBins ? UINT_MAX : nBins );

	// Find the smallest prime greater than our estimate
	int iPrime = 0;
	while( nBinsEstimate > s_anPrimes[iPrime] )
	{
		iPrime++;
	}

	if( s_anPrimes[iPrime] == UINT_MAX )
	{
		return( nBinsEstimate );
	}
	else
	{
		return( s_anPrimes[iPrime] );
	}
}

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CNode* CLgnMap< K, V, KTraits, VTraits >::CreateNode( 
	KINARGTYPE key, UINT iBin, UINT nHash )
{
	CNode* pNode;

	if( m_ppBins == NULL )
	{
		bool bSuccess;

		bSuccess = InitHashTable( m_nBins );
		if( !bSuccess )
		{
			LGN::LgnThrow( E_OUTOFMEMORY );
		}
	}

	pNode = NewNode( key, iBin, nHash );

	return( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
POSITION CLgnMap< K, V, KTraits, VTraits >::GetStartPosition() const throw()
{
	if( IsEmpty() )
	{
		return( NULL );
	}

	for( UINT iBin = 0; iBin < m_nBins; iBin++ )
	{
		if( m_ppBins[iBin] != NULL )
		{
			return( POSITION( m_ppBins[iBin] ) );
		}
	}
	LGNASSERT( false );

	return( NULL );
}

template< typename K, typename V, class KTraits, class VTraits >
POSITION CLgnMap< K, V, KTraits, VTraits >::SetAt( KINARGTYPE key, VINARGTYPE value )
{
	CNode* pNode;
	UINT iBin;
	UINT nHash;
	CNode* pPrev;

	pNode = GetNode( key, iBin, nHash, pPrev );
	if( pNode == NULL )
	{
		pNode = CreateNode( key, iBin, nHash );
		_LGNTRY
		{
			pNode->m_value = value;
		}
		_LGNCATCHALL()
		{
			RemoveAtPos( POSITION( pNode ) );
			_LGNRETHROW;
		}
	}
	else
	{
		pNode->m_value = value;
	}

	return( POSITION( pNode ) );
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::SetValueAt( POSITION pos, VINARGTYPE value )
{
	LGNASSERT( pos != NULL );

	CNode* pNode = static_cast< CNode* >( pos );
	pNode->m_value = value;
}

template< typename K, typename V, class KTraits, class VTraits >
CLgnMap< K, V, KTraits, VTraits >::CLgnMap( UINT nBins, float fOptimalLoad, 
	float fLoThreshold, float fHiThreshold, UINT nBlockSize ) throw() :
	m_ppBins( NULL ),
	m_nBins( nBins ),
	m_nElements( 0 ),
	m_nLockCount( 0 ),  // Start unlocked
	m_fOptimalLoad( fOptimalLoad ),
	m_fLoThreshold( fLoThreshold ),
	m_fHiThreshold( fHiThreshold ),
	m_nHiRehashThreshold( UINT_MAX ),
	m_nLoRehashThreshold( 0 ),
	m_pBlocks( NULL ),
	m_pFree( NULL ),
	m_nBlockSize( nBlockSize )
{
	LGNASSERT( nBins > 0 );
	LGNASSERT( nBlockSize > 0 );

	SetOptimalLoad( fOptimalLoad, fLoThreshold, fHiThreshold, false );
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::SetOptimalLoad( float fOptimalLoad, float fLoThreshold,
	float fHiThreshold, bool bRehashNow )
{
	LGNASSERT( fOptimalLoad > 0 );
	LGNASSERT( (fLoThreshold >= 0) && (fLoThreshold < fOptimalLoad) );
	LGNASSERT( fHiThreshold > fOptimalLoad );

	m_fOptimalLoad = fOptimalLoad;
	m_fLoThreshold = fLoThreshold;
	m_fHiThreshold = fHiThreshold;

	UpdateRehashThresholds();

	if( bRehashNow && ((m_nElements > m_nHiRehashThreshold) || 
		(m_nElements < m_nLoRehashThreshold)) )
	{
		Rehash( PickSize( m_nElements ) );
	}
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::UpdateRehashThresholds() throw()
{
	m_nHiRehashThreshold = size_t( m_fHiThreshold*m_nBins );
	m_nLoRehashThreshold = size_t( m_fLoThreshold*m_nBins );
	if( m_nLoRehashThreshold < 17 )
	{
		m_nLoRehashThreshold = 0;
	}
}

template< typename K, typename V, class KTraits, class VTraits >
bool CLgnMap< K, V, KTraits, VTraits >::InitHashTable( UINT nBins, bool bAllocNow )
{
	LGNASSERT( m_nElements == 0 );
	LGNASSERT( nBins > 0 );

	if( m_ppBins != NULL )
	{
		delete[] m_ppBins;
		m_ppBins = NULL;
	}

	if( bAllocNow )
	{
		LGNTRY( m_ppBins = new CNode*[nBins] );
		if( m_ppBins == NULL )
		{
			return false;
		}
		LGN::API::memset( m_ppBins, 0, sizeof( CNode* )*nBins );
	}
	m_nBins = nBins;

	UpdateRehashThresholds();

	return true;
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::RemoveAll() throw()
{
	DisableAutoRehash();
	if( m_ppBins != NULL )
	{
		for( UINT iBin = 0; iBin < m_nBins; iBin++ )
		{
			CNode* pNext;

			pNext = m_ppBins[iBin];
			while( pNext != NULL )
			{
				CNode* pKill;

				pKill = pNext;
				pNext = pNext->m_pNext;
				FreeNode( pKill );
			}
		}
	}

	delete[] m_ppBins;
	m_ppBins = NULL;
	m_nElements = 0;

	if( !IsLocked() )
	{
		InitHashTable( PickSize( m_nElements ), false );
	}

	FreePlexes();
	EnableAutoRehash();
}

template< typename K, typename V, class KTraits, class VTraits >
CLgnMap< K, V, KTraits, VTraits >::~CLgnMap() throw()
{
	RemoveAll();
}

#pragma push_macro("new")
#undef new

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CNode* CLgnMap< K, V, KTraits, VTraits >::NewNode( 
	KINARGTYPE key, UINT iBin, UINT nHash )
{
	CNode* pNewNode;

	if( m_pFree == NULL )
	{
		CLgnPlex* pPlex;
		CNode* pNode;

		pPlex = CLgnPlex::Create( m_pBlocks, m_nBlockSize, sizeof( CNode ) );
		if( pPlex == NULL )
		{
			LGN::LgnThrow( E_OUTOFMEMORY );
		}
		pNode = (CNode*)pPlex->data();
		pNode += m_nBlockSize-1;
		for( int iBlock = m_nBlockSize-1; iBlock >= 0; iBlock-- )
		{
			pNode->m_pNext = m_pFree;
			m_pFree = pNode;
			pNode--;
		}
	}
	LGNASSERT( m_pFree != NULL );
	pNewNode = m_pFree;
	m_pFree = pNewNode->m_pNext;

	_LGNTRY
	{
		::new( pNewNode ) CNode( key, nHash );
	}
	_LGNCATCHALL()
	{
		pNewNode->m_pNext = m_pFree;
		m_pFree = pNewNode;

		_LGNRETHROW;
	}
	m_nElements++;

	pNewNode->m_pNext = m_ppBins[iBin];
	m_ppBins[iBin] = pNewNode;

	if( (m_nElements > m_nHiRehashThreshold) && !IsLocked() )
	{
		Rehash( PickSize( m_nElements ) );
	}

	return( pNewNode );
}

#pragma pop_macro("new")

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::FreeNode( CNode* pNode ) throw()
{
	LGNASSERT( pNode != NULL );

	pNode->~CNode();
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;

	LGNASSERT( m_nElements > 0 );
	m_nElements--;

	if( (m_nElements < m_nLoRehashThreshold) && !IsLocked() )
	{
		Rehash( PickSize( m_nElements ) );
	}

	if( m_nElements == 0 )
	{
		FreePlexes();
	}
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::FreePlexes() throw()
{
	m_pFree = NULL;
	if( m_pBlocks != NULL )
	{
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;
	}
}

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CNode* CLgnMap< K, V, KTraits, VTraits >::GetNode(
	KINARGTYPE key, UINT& iBin, UINT& nHash, CNode*& pPrev ) const throw()
{
	CNode* pFollow;

	nHash = KTraits::Hash( key );
	iBin = nHash%m_nBins;

	if( m_ppBins == NULL )
	{
		return( NULL );
	}

	pFollow = NULL;
	pPrev = NULL;
	for( CNode* pNode = m_ppBins[iBin]; pNode != NULL; pNode = pNode->m_pNext )
	{
		if( (pNode->GetHash() == nHash) && KTraits::CompareElements( pNode->m_key, key ) )
		{
			pPrev = pFollow;
			return( pNode );
		}
		pFollow = pNode;
	}

	return( NULL );
}

template< typename K, typename V, class KTraits, class VTraits >
bool CLgnMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key, VOUTARGTYPE value ) const
{
	UINT iBin;
	UINT nHash;
	CNode* pNode;
	CNode* pPrev;

	pNode = GetNode( key, iBin, nHash, pPrev );
	if( pNode == NULL )
	{
		return( false );
	}

	value = pNode->m_value;

	return( true );
}

template< typename K, typename V, class KTraits, class VTraits >
const typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key ) const throw()
{
	UINT iBin;
	UINT nHash;
	CNode* pNode;
	CNode* pPrev;

	pNode = GetNode( key, iBin, nHash, pPrev );

	return( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::Lookup( KINARGTYPE key ) throw()
{
	UINT iBin;
	UINT nHash;
	CNode* pNode;
	CNode* pPrev;

	pNode = GetNode( key, iBin, nHash, pPrev );

	return( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
bool CLgnMap< K, V, KTraits, VTraits >::RemoveKey( KINARGTYPE key ) throw()
{
	CNode* pNode;
	UINT iBin;
	UINT nHash;
	CNode* pPrev;

	pPrev = NULL;
	pNode = GetNode( key, iBin, nHash, pPrev );
	if( pNode == NULL )
	{
		return( false );
	}

	RemoveNode( pNode, pPrev );

	return( true );
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::RemoveNode( CNode* pNode, CNode* pPrev ) throw()
{
	UINT iBin;

	LGNASSERT( pNode != NULL );

	iBin = pNode->GetHash()%m_nBins;
	if( pPrev == NULL )
	{
		LGNASSERT( m_ppBins[iBin] == pNode );
		m_ppBins[iBin] = pNode->m_pNext;
	}
	else
	{
		LGNASSERT( pPrev->m_pNext == pNode );
		pPrev->m_pNext = pNode->m_pNext;
	}
	FreeNode( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::RemoveAtPos( POSITION pos ) throw()
{
	CNode* pNode;
	CNode* pPrev;
	UINT iBin;

	LGNASSERT( pos != NULL );
	pNode = static_cast< CNode* >( pos );
	iBin = pNode->GetHash()%m_nBins;

	LGNASSERT( m_ppBins[iBin] != NULL );
	if( pNode == m_ppBins[iBin] )
	{
		pPrev = NULL;
	}
	else
	{
		pPrev = m_ppBins[iBin];
		while( pPrev->m_pNext != pNode )
		{
			pPrev = pPrev->m_pNext;
			LGNASSERT( pPrev != NULL );
		}
	}
	RemoveNode( pNode, pPrev );
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::Rehash( UINT nBins )
{
	CNode** ppBins = NULL;

	if( nBins == 0 )
	{
		nBins = PickSize( m_nElements );
	}

	if( nBins == m_nBins )
	{
		return;
	}

	LGNTRACE(LGN_TRACE_CATEGORY_DEF, TraceLevel::Debug, TEXT("CLgnMap::Rehash: %u bins\n"), nBins );

	if( m_ppBins == NULL )
	{
		// Just set the new number of bins
		InitHashTable( nBins, false );
		return;
	}

	LGNTRY(ppBins = new CNode*[nBins]);
	if (ppBins == NULL)
	{
		LGN::LgnThrow( E_OUTOFMEMORY );
	}

	LGN::API::memset( ppBins, 0, nBins*sizeof( CNode* ) );

	// Nothing gets copied.  We just rewire the old nodes
	// into the new bins.
	for( UINT iSrcBin = 0; iSrcBin < m_nBins; iSrcBin++ )
	{
		CNode* pNode;

		pNode = m_ppBins[iSrcBin];
		while( pNode != NULL )
		{
			CNode* pNext;
			UINT iDestBin;

			pNext = pNode->m_pNext;  // Save so we don't trash it
			iDestBin = pNode->GetHash()%nBins;
			pNode->m_pNext = ppBins[iDestBin];
			ppBins[iDestBin] = pNode;

			pNode = pNext;
		}
	}

	delete[] m_ppBins;
	m_ppBins = ppBins;
	m_nBins = nBins;

	UpdateRehashThresholds();
}

template< typename K, typename V, class KTraits, class VTraits >
void CLgnMap< K, V, KTraits, VTraits >::GetNextAssoc( POSITION& pos, KOUTARGTYPE key,
	VOUTARGTYPE value ) const
{
	CNode* pNode;
	CNode* pNext;

	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;
	pNext = FindNextNode( pNode );

	pos = POSITION( pNext );
	key = pNode->m_key;
	value = pNode->m_value;
}

template< typename K, typename V, class KTraits, class VTraits >
const typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::GetNext( POSITION& pos ) const throw()
{
	CNode* pNode;
	CNode* pNext;

	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;
	pNext = FindNextNode( pNode );

	pos = POSITION( pNext );

	return( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CPair* CLgnMap< K, V, KTraits, VTraits >::GetNext( 
	POSITION& pos ) throw()
{
	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	CNode* pNode = static_cast< CNode* >( pos );
	CNode* pNext = FindNextNode( pNode );

	pos = POSITION( pNext );

	return( pNode );
}

template< typename K, typename V, class KTraits, class VTraits >
const K& CLgnMap< K, V, KTraits, VTraits >::GetNextKey( POSITION& pos ) const throw()
{
	CNode* pNode;
	CNode* pNext;

	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;
	pNext = FindNextNode( pNode );

	pos = POSITION( pNext );

	return( pNode->m_key );
}

template< typename K, typename V, class KTraits, class VTraits >
const V& CLgnMap< K, V, KTraits, VTraits >::GetNextValue( POSITION& pos ) const throw()
{
	CNode* pNode;
	CNode* pNext;

	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;
	pNext = FindNextNode( pNode );

	pos = POSITION( pNext );

	return( pNode->m_value );
}

template< typename K, typename V, class KTraits, class VTraits >
V& CLgnMap< K, V, KTraits, VTraits >::GetNextValue( POSITION& pos ) throw()
{
	CNode* pNode;
	CNode* pNext;

	LGNASSERT( m_ppBins != NULL );
	LGNASSERT( pos != NULL );

	pNode = (CNode*)pos;
	pNext = FindNextNode( pNode );

	pos = POSITION( pNext );

	return( pNode->m_value );
}

template< typename K, typename V, class KTraits, class VTraits >
typename CLgnMap< K, V, KTraits, VTraits >::CNode* CLgnMap< K, V, KTraits, VTraits >::FindNextNode( CNode* pNode ) const throw()
{
	CNode* pNext;
	
	if(pNode == NULL)
	{
		LGNASSERT(false);
		return NULL;
	}
	
	if( pNode->m_pNext != NULL )
	{
		pNext = pNode->m_pNext;
	}
	else
	{
		UINT iBin;

		pNext = NULL;
		iBin = (pNode->GetHash()%m_nBins)+1;
		while( (pNext == NULL) && (iBin < m_nBins) )
		{
			if( m_ppBins[iBin] != NULL )
			{
				pNext = m_ppBins[iBin];
			}

			iBin++;
		}
	}

	return( pNext );
}


//! 来源 atlcoll.h，同<B>ATL::CMapToSmartPtr</B>，具体参考MSDN
template< typename K, typename V, class KTraits = CElementTraits< K > >
class CMapToSmartPtr :
	public CLgnMap< K, LGN::CSmartPtr< V >, KTraits, CSmartPtrElementTraits< V > >
{
public:
	CMapToSmartPtr( UINT nBins = 17 ) throw();

private:
	// Private to prevent use
	CMapToSmartPtr( const CMapToSmartPtr& ) throw();
	CMapToSmartPtr& operator=( const CMapToSmartPtr& ) throw();
};

template< typename K, typename V, class KTraits >
inline CMapToSmartPtr< K, V, KTraits >::CMapToSmartPtr( UINT nBins ) throw() :
	CLgnMap< K, LGN::CSmartPtr< V >, KTraits, CSmartPtrElementTraits< V > >( nBins )
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////

//! 线程安全智能对象引用列表
/*! 在线程安全临界区保护下，维护（增删改查）智能对象引用列表，内部使用CSmartPtrList */
template< typename T >
class CSafeHandleList
{
public:
	typedef LGN::CSmartPtr< T >& INARGTYPE; //!< 输入参数类型
	typedef LGN::CSmartPtr< T >& OUTARGTYPE; //!< 输出参数类型

	//! 构造函数，初始成员变量
	/*!	\param[in] nBlockSize 按该值来扩容列表*/
	CSafeHandleList( UINT nBlockSize = 10 ) throw() :
		m_list( nBlockSize )
	{
	}

	//! 增加一个对象
	/*!	\param[in] element 对象引用
		\return 该对象的句柄（对象指针）*/
	HANDLE Add(INARGTYPE element)
	{
		CCriticalSectionLock _lock( m_csList );
		m_list.AddTail( element );
		return (T*)element;
	}

	//! 移除一个对象
	/*!	\param[in] hHandle 对象句柄（对象指针）
		\return 返回成功：该对象存在，并从列表中移出*/
	bool Remove(HANDLE hHandle)
	{
		LGN::CSmartPtr< T > element;
		element.Attach( (T *)hHandle ); // 防止非职能指针操作
		CCriticalSectionLock _lock( m_csList );
		POSITION pos = m_list.Find(element);
		if (pos != NULL)
			m_list.RemoveAt(pos);
		element.Detach();
		return (pos != NULL);
	}

	//! 在列表中查询对象
	/*!	\param[in] hHandle 对象句柄（对象指针）
		\param[out] ptrObject 对象引用
		\return 返回成功：列表中存在该对象，并付值到\a ptrObject*/
	bool Lookup(HANDLE hHandle, OUTARGTYPE ptrObject)
	{
		LGN::CSmartPtr< T > element;
		element.Attach( (T *)hHandle ); // 防止非职能指针操作
		CCriticalSectionLock _lock( m_csList );
		POSITION pos = m_list.Find(element);
		if (pos != NULL)
			ptrObject = element;
		element.Detach();
		return (pos != NULL);
	}


protected:
	CSmartPtrList< T > m_list;	//!< 智能对象列表
	CAutoCriticalSection m_csList; //!< 线程安全临界区对象
};


#ifdef __LGNSTR_H__
typedef CLgnArray< CString > CStringArray;
typedef CLgnList< CString > CStringList;
#endif

}; // namespace LGN

#endif
